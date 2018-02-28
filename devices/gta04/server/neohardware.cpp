/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2012 Radek Polak.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#include "neohardware.h"

#include <QSocketNotifier>
#include <QTimer>
#include <QLabel>
#include <QDesktopWidget>
#include <QProcess>
#include <QSettings>
#include <QPowerSourceProvider>

#include <qcontentset.h>
#include <qtopiaapplication.h>
#include <qtopialog.h>
#include <qtopiaipcadaptor.h>

#include <qbootsourceaccessory.h>
#include <qtopiaipcenvelope.h>

#include <qtopiaserverapplication.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/input.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <QTcpSocket>
#include <QtDebug>

QTOPIA_TASK(NeoHardware, NeoHardware);

// Setup netlink socket for watching usb cable and battery events
static int openNetlink()
{
    struct sockaddr_nl snl;
    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    snl.nl_groups = 1;
    snl.nl_groups = 0x1;

    int fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (fd == -1) {
        qLog(Hardware) << "error getting uevent socket: " << strerror(errno);
        return -1;
    }
    if (bind(fd, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl)) < 0) {
        qLog(Hardware) << "uevent bind failed: " << strerror(errno);
        return -1;
    }
    return fd;
}

NeoHardware::NeoHardware()
:
ac(QPowerSource::Wall, "PrimaryAC", this)
    , battery(QPowerSource::Battery, "NeoBattery", this)
    , batteryVso("/UI/Battery", this)
    , vsoPortableHandsfree("/Hardware/Accessories/PortableHandsfree")
    , chargeLog("/var/log/charging")
    , lastLogDt()
    , ueventSocket(this)
    , timer(this)
    , chargingLogInterval(300)
    , maxChargeCurrent(-1)
{
    qLog(Hardware) << "gta04 hardware";

    int netlinkFd = openNetlink();
    if (netlinkFd >= 0) {
        ueventSocket.setSocketDescriptor(netlinkFd);
        connect(&ueventSocket, SIGNAL(readyRead()), this, SLOT(uevent()));
    }

    hasSmartBattery =
        QFile::exists("/sys/class/power_supply/bq27000-battery/status");

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    timer.start(30 * 1000);

    QTimer::singleShot(1, this, SLOT(updateStatus()));

    adaptor = new QtopiaIpcAdaptor("QPE/NeoHardware");
    audioMgr = new QtopiaIpcAdaptor("QPE/AudioStateManager", this);

    QtopiaIpcAdaptor::connect(adaptor, MESSAGE(headphonesInserted(bool)),
                              this, SLOT(headphonesInserted(bool)));

    // Y sets usb charging limit to 600mA which can be too high and causes
    // charging voltage drops. We will set limit manually to prevent this.
    //qWriteFile("/sys/module/twl4030_charger/parameters/allow_usb", "N");
    
    QSettings cfg("Trolltech", "qpe");
    cfg.beginGroup("Charging");
    chargingLogInterval = cfg.value("LogInterval", 300).toInt();
}

NeoHardware::~NeoHardware()
{
}

void NeoHardware::setMaxChargeCurrent(int newValue)
{
    if (newValue < 100000)
        newValue = 100000;
    if (newValue > MAX_CURRENT)
        newValue = MAX_CURRENT;
    if (maxChargeCurrent == newValue)
        return;

    char buf[7];
    maxChargeCurrent = newValue;
    sprintf(buf, "%d", maxChargeCurrent);
    qWriteFile("/sys/class/power_supply/twl4030_usb/input_current_limit", buf);
}

// Parse uevent string and return given attribute value. Example uevent file:
// POWER_SUPPLY_NAME=bq27000-battery
// POWER_SUPPLY_STATUS=Full
// POWER_SUPPLY_PRESENT=1
// POWER_SUPPLY_VOLTAGE_NOW=65535000
// POWER_SUPPLY_CURRENT_NOW=-11697997
// POWER_SUPPLY_CAPACITY_LEVEL=Full
// POWER_SUPPLY_TEMP=161106
// POWER_SUPPLY_TECHNOLOGY=Li-ion
// POWER_SUPPLY_CHARGE_NOW=11697997
// POWER_SUPPLY_CHARGE_FULL_DESIGN=11652480
// POWER_SUPPLY_CYCLE_COUNT=65535
//
// Btw this is uevent file for case of uncalibrated battery.
//
// The argument name must contain even the '=' char
static QByteArray getAttr(const char *name, QByteArray & uevent)
{
    int index = uevent.indexOf(name);
    if (index < 0)
        return QByteArray();

    index += strlen(name);
    int end = uevent.indexOf('\n', index + 1);
    return uevent.mid(index, end - index);
}

static int getIntAttr(const char *name, QByteArray & uevent)
{
    QByteArray str = getAttr(name, uevent);
    if (str.isEmpty())
        return -1;
    return str.toInt();
}

void NeoHardware::logCharge(QDateTime now, int chargeNow)
{
    if (lastLogDt.secsTo(now) < chargingLogInterval)
        return;

    lastLogDt = now;

    if (!chargeLog.exists())
        return;

    QString newEntry =
        now.toString("yyyy-MM-dd hh:mm:ss") + "\t" +
        QString::number(chargeNow) + "\n";

    if (!chargeLog.open(QIODevice::WriteOnly | QIODevice::Append))
        return;

    chargeLog.write(newEntry.toLatin1().constData());
    chargeLog.close();
    
    // When user selects fast updates, use timer to update status because
    // uevent does not trigger that fast.
    if (chargingLogInterval < 300)
        QTimer::singleShot(1000 * chargingLogInterval, this, SLOT(updateStatus()));
}

void NeoHardware::updateStatus()
{
    QDateTime now = QDateTime::currentDateTime();

    // Determine charging via USB
    QByteArray twlVbus =
        qReadFile("/sys/class/power_supply/twl4030_usb/online");

    bool chargerOn = twlVbus.contains("1");
    if (chargerOn)
        ac.setAvailability(QPowerSource::Available);
    else
        ac.setAvailability(QPowerSource::NotAvailable);

    battery.setCharging(chargerOn);

    // Update battery status
    QByteArray uevent =
        qReadFile("/sys/class/power_supply/bq27000-battery/uevent");

    int timeToEmpty = getIntAttr("POWER_SUPPLY_TIME_TO_EMPTY_NOW=", uevent);
    if (timeToEmpty >= 0)
        battery.setTimeRemaining(timeToEmpty / 60);

    int capacity = getIntAttr("POWER_SUPPLY_CAPACITY=", uevent);
    int chargeNow = getIntAttr("POWER_SUPPLY_CHARGE_NOW=", uevent);
    if (capacity < 0) {
        // Handle uncalibrated battery - it does not have POWER_SUPPLY_CAPACITY
        // and we compute capacity from charge_now and charge_full_design.
        // http://lists.goldelico.com/pipermail/gta04-owner/2013-February/003903.html        
        int chargeFullDesign =
            getIntAttr("POWER_SUPPLY_CHARGE_FULL_DESIGN=", uevent);
        capacity = (chargeNow * 100) / chargeFullDesign;
    }

    if (capacity > 0)
        battery.setCharge(capacity);

    int currentNow = getIntAttr("POWER_SUPPLY_CURRENT_NOW=", uevent);
    batteryVso.setAttribute("current_now", QString::number(currentNow / 1000));
#if 0
    // handled by kernel, not needed anymore
    // Now we will try to set max_current so that phone charges reliably.
    int chargerVoltage = -1;
    if (chargerOn) {

        if (maxChargeCurrent < 0)
            setMaxChargeCurrent(INIT_CURRENT);

        QByteArray chargerUevent =
            qReadFile("/sys/class/power_supply/twl4030_usb/uevent");
        chargerVoltage = getIntAttr("POWER_SUPPLY_VOLTAGE_NOW=", chargerUevent);

        // Warn if charging voltage drops too low
        if (chargerVoltage < 4500000 && chargerVoltage > 0)
            qWarning() << "Charging voltage dropped to " << chargerVoltage <<
                ", charging might not restart when battery gets low";

        // Battery discharges
        if (currentNow > 0 || capacity <= 90) {
            // Increase charging current until 600mA, but make sure that
            // voltage wont drop under 4.5V, for more info see:
            // http://lists.goldelico.com/pipermail/gta04-owner/2013-September/004963.html
            if (chargerVoltage > 4600000 && chargerVoltage != oldChargerVoltage) {
                setMaxChargeCurrent(maxChargeCurrent + CURRENT_PLUS);
                QTimer::singleShot(200, this, SLOT(updateStatus()));
            }
        } else if (currentNow < -50000 && oldChargeNow != chargeNow) {  // Battery is finishing charging
            setMaxChargeCurrent(maxChargeCurrent + currentNow / 2); // slow charging down under 50mA
        }

        oldChargerVoltage = chargerVoltage;
    } else if (maxChargeCurrent > INIT_CURRENT) {
        maxChargeCurrent = -1;
    }
    oldChargeNow = chargeNow;
#endif

    // Charging log
    logCharge(now, chargeNow);
}

#define UEVENT_BUFFER_SIZE 1024
void NeoHardware::uevent()
{
    char buffer[1024];
    if (ueventSocket.read(buffer, UEVENT_BUFFER_SIZE) <= 0) {
        return;
    }
    if (strstr(buffer, "twl4030") || strstr(buffer, "bq27000-battery")) {
        QTimer::singleShot(1000, this, SLOT(updateStatus()));   // vbus updates < 1s
        QTimer::singleShot(2000, this, SLOT(updateStatus()));   // but sometimes needs 2s when changing on->off
        QTimer::singleShot(10000, this, SLOT(updateStatus()));  // battery charging needs 10s
        QTimer::singleShot(20000, this, SLOT(updateStatus()));  // try also after 20s in case status at 10s was not yet correct
    }
}

void NeoHardware::shutdownRequested()
{
    qLog(PowerManagement) << __PRETTY_FUNCTION__;
    QtopiaServerApplication::instance()->
        shutdown(QtopiaServerApplication::ShutdownSystem);
}

void NeoHardware::headphonesInserted(bool b)
{
    qLog(Hardware) << __PRETTY_FUNCTION__ << b;
    vsoPortableHandsfree.setAttribute("Present", b);
    vsoPortableHandsfree.sync();
    if (b) {
        QByteArray mode("Headphone");
        audioMgr->send("setProfile(QByteArray)", mode);
    } else {
        QByteArray mode("MediaSpeaker");
        audioMgr->send("setProfile(QByteArray)", mode);
    }
}
