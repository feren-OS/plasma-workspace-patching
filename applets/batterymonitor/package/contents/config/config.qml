/***************************************************************************
 *   SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>                            *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

import QtQuick 2.0

import org.kde.plasma.configuration 2.0

ConfigModel {
    ConfigCategory {
         name: i18n("General")
         icon: "battery"
         source: "ConfigGeneral.qml"
    }
}
