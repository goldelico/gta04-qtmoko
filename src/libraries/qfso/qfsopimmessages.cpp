/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsopimmessages -c QFsoPIMMessages specs/xml/org.freesmartphone.PIM.Messages.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "qfsopimmessages.h"

/*
 * Implementation of interface class QFsoPIMMessages
 */

QFsoPIMMessages::QFsoPIMMessages(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QFsoDbusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

QFsoPIMMessages::~QFsoPIMMessages()
{
}

