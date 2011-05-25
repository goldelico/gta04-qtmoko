/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsopimfields -c QFsoPIMFields specs/xml/org.freesmartphone.PIM.Fields.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef QFSOPIMFIELDS_H_1306329414
#define QFSOPIMFIELDS_H_1306329414

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "qfsostringmap.h"

/*
 * Proxy class for interface org.freesmartphone.PIM.Fields
 */
class QFsoPIMFields: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freesmartphone.PIM.Fields"; }

public:
    QFsoPIMFields(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~QFsoPIMFields();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> AddField(const QString &name, const QString &type)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(name) << qVariantFromValue(type);
        return asyncCallWithArgumentList(QLatin1String("AddField"), argumentList);
    }

    inline QDBusPendingReply<> DeleteField(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(name);
        return asyncCallWithArgumentList(QLatin1String("DeleteField"), argumentList);
    }

    inline QDBusPendingReply<QString> GetType(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(name);
        return asyncCallWithArgumentList(QLatin1String("GetType"), argumentList);
    }

    inline QDBusPendingReply<QFsoStringMap> ListFields()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("ListFields"), argumentList);
    }

    inline QDBusPendingReply<QStringList> ListFieldsWithType(const QString &type)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(type);
        return asyncCallWithArgumentList(QLatin1String("ListFieldsWithType"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace org {
  namespace freesmartphone {
    namespace PIM {
      typedef ::QFsoPIMFields Fields;
    }
  }
}
#endif
