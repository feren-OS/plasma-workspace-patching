/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: MIT
*/

#ifndef SESSIONSPRIVATEPLUGIN_H
#define SESSIONSPRIVATEPLUGIN_H

#include <QQmlExtensionPlugin>

class SessionsPrivatePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif // SESSIONSPRIVATEPLUGIN_H
