/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsopimcallquery -c QFsoPIMCallQuery specs/xml/org.freesmartphone.PIM.CallQuery.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "qfsopimcallquery.h"

/*
 * Implementation of interface class QFsoPIMCallQuery
 */

QFsoPIMCallQuery::QFsoPIMCallQuery(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
    QFsoVariantMapList::registerMetaType();
}

QFsoPIMCallQuery::~QFsoPIMCallQuery()
{
}

