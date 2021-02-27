/*
 *   SPDX-FileCopyrightText: 2020 Konrad Materka <materka@gmail.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick 2.14
import QtQuick.Controls 2.14 as QQC2
import QtQuick.Layouts 1.13

import org.kde.plasma.core 2.1 as PlasmaCore

import org.kde.kirigami 2.13 as Kirigami

ColumnLayout {
    property bool cfg_scaleIconsToFit

    Kirigami.FormLayout {
        Layout.fillHeight: true

        QQC2.RadioButton {
            Kirigami.FormData.label: i18nc("The arrangement of system tray icons in the Panel", "Panel icon size:")
            text: i18n("Small")
            checked: cfg_scaleIconsToFit == false
            onToggled: cfg_scaleIconsToFit = !checked
        }
        QQC2.RadioButton {
            id: automaticRadioButton
            text: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? i18n("Scale with Panel height")
                                                                      : i18n("Scale with Panel width")
            checked: cfg_scaleIconsToFit == true
            onToggled: cfg_scaleIconsToFit = checked
        }
    }
}
