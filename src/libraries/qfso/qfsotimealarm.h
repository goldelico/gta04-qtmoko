/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsotimealarm -c QFsoTimeAlarm specs/xml/org.freesmartphone.Time.Alarm.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef QFSOTIMEALARM_H_1306329414
#define QFSOTIMEALARM_H_1306329414

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "qfsowakeupalarm.h"

/*
 * Proxy class for interface org.freesmartphone.Time.Alarm
 */
class QFsoTimeAlarm: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freesmartphone.Time.Alarm"; }

public:
    QFsoTimeAlarm(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~QFsoTimeAlarm();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> AddAlarm(const QString &busname, int timestamp)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(busname) << qVariantFromValue(timestamp);
        return asyncCallWithArgumentList(QLatin1String("AddAlarm"), argumentList);
    }

    inline QDBusPendingReply<> ClearAlarms(const QString &busname)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(busname);
        return asyncCallWithArgumentList(QLatin1String("ClearAlarms"), argumentList);
    }

    inline QDBusPendingReply<QFsoWakeupAlarm> ListAlarms()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("ListAlarms"), argumentList);
    }

    inline QDBusPendingReply<> RemoveAlarm(const QString &busname, int timestamp)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(busname) << qVariantFromValue(timestamp);
        return asyncCallWithArgumentList(QLatin1String("RemoveAlarm"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace org {
  namespace freesmartphone {
    namespace Time {
      typedef ::QFsoTimeAlarm Alarm;
    }
  }
}
#endif
