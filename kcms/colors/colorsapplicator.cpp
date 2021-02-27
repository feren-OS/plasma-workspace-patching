/*
   SPDX-FileCopyrightText: 2021 Dan Leinir Turthra Jensen <admin@leinir.dk>

   SPDX-License-Identifier: LGPL-2.0-only
*/

#include "colorssettings.h"
#include "colorsmodel.h"

#include "../krdb/krdb.h"

#include <KColorScheme>
#include <KConfig>

#include <QDBusConnection>
#include <QDBusMessage>

static void copyEntry(KConfigGroup &from, KConfigGroup &to, const QString &entry)
{
    if (from.hasKey(entry)) {
        to.writeEntry(entry, from.readEntry(entry));
    }
}

void applyScheme(ColorsSettings *settings, ColorsModel *model)
{
    const QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("color-schemes/%1.colors").arg(model->selectedScheme()));

    // Using KConfig::SimpleConfig because otherwise Header colors won't be
    // rewritten when a new color scheme is loaded.
    KSharedConfigPtr config = KSharedConfig::openConfig(path, KConfig::SimpleConfig);

    const QStringList colorSetGroupList{QStringLiteral("Colors:View"),
                                        QStringLiteral("Colors:Window"),
                                        QStringLiteral("Colors:Button"),
                                        QStringLiteral("Colors:Selection"),
                                        QStringLiteral("Colors:Tooltip"),
                                        QStringLiteral("Colors:Complementary"),
                                        QStringLiteral("Colors:Header")};

    const QStringList colorSetKeyList{QStringLiteral("BackgroundNormal"),
                                      QStringLiteral("BackgroundAlternate"),
                                      QStringLiteral("ForegroundNormal"),
                                      QStringLiteral("ForegroundInactive"),
                                      QStringLiteral("ForegroundActive"),
                                      QStringLiteral("ForegroundLink"),
                                      QStringLiteral("ForegroundVisited"),
                                      QStringLiteral("ForegroundNegative"),
                                      QStringLiteral("ForegroundNeutral"),
                                      QStringLiteral("ForegroundPositive"),
                                      QStringLiteral("DecorationFocus"),
                                      QStringLiteral("DecorationHover")};

    for (auto item : colorSetGroupList) {
        settings->config()->deleteGroup(item);

        KConfigGroup sourceGroup(config, item);
        KConfigGroup targetGroup(settings->config(), item);

        for (auto entry : colorSetKeyList) {
            copyEntry(sourceGroup, targetGroup, entry);
        }

        if (sourceGroup.hasGroup("Inactive")) {
            sourceGroup = sourceGroup.group("Inactive");
            targetGroup = targetGroup.group("Inactive");

            for (auto entry : colorSetKeyList) {
                copyEntry(sourceGroup, targetGroup, entry);
            }
        }
    }

    KConfigGroup groupWMTheme(config, "WM");
    KConfigGroup groupWMOut(settings->config(), "WM");
    KColorScheme inactiveHeaderColorScheme(QPalette::Inactive, KColorScheme::Header, config);

    const QStringList colorItemListWM{QStringLiteral("activeBackground"),
                                      QStringLiteral("activeForeground"),
                                      QStringLiteral("inactiveBackground"),
                                      QStringLiteral("inactiveForeground"),
                                      QStringLiteral("activeBlend"),
                                      QStringLiteral("inactiveBlend")};

    const QVector<QColor> defaultWMColors{KColorScheme(QPalette::Normal, KColorScheme::Header, config).background().color(),
                                          KColorScheme(QPalette::Normal, KColorScheme::Header, config).foreground().color(),
                                          inactiveHeaderColorScheme.background().color(),
                                          inactiveHeaderColorScheme.foreground().color(),
                                          KColorScheme(QPalette::Normal, KColorScheme::Header, config).background().color(),
                                          inactiveHeaderColorScheme.background().color()};

    int i = 0;
    for (const QString &coloritem : colorItemListWM) {
        groupWMOut.writeEntry(coloritem, groupWMTheme.readEntry(coloritem, defaultWMColors.value(i)));
        ++i;
    }

    const QStringList groupNameList{QStringLiteral("ColorEffects:Inactive"), QStringLiteral("ColorEffects:Disabled")};

    const QStringList effectList{QStringLiteral("Enable"),
                                 QStringLiteral("ChangeSelectionColor"),
                                 QStringLiteral("IntensityEffect"),
                                 QStringLiteral("IntensityAmount"),
                                 QStringLiteral("ColorEffect"),
                                 QStringLiteral("ColorAmount"),
                                 QStringLiteral("Color"),
                                 QStringLiteral("ContrastEffect"),
                                 QStringLiteral("ContrastAmount")};

    for (const QString &groupName : groupNameList) {
        KConfigGroup groupEffectOut(settings->config(), groupName);
        KConfigGroup groupEffectTheme(config, groupName);

        for (const QString &effect : effectList) {
            groupEffectOut.writeEntry(effect, groupEffectTheme.readEntry(effect));
        }
    }

    settings->config()->sync();

    bool applyToAlien{true};
    {
        KConfig cfg(QStringLiteral("kcmdisplayrc"), KConfig::NoGlobals);
        KConfigGroup group(settings->config(), "General");
        group = KConfigGroup(&cfg, "X11");
        applyToAlien = group.readEntry("exportKDEColors", applyToAlien);
    }
    runRdb(KRdbExportQtColors | KRdbExportGtkTheme | (applyToAlien ? KRdbExportColors : 0));

    QDBusMessage message =
        QDBusMessage::createSignal(QStringLiteral("/KGlobalSettings"), QStringLiteral("org.kde.KGlobalSettings"), QStringLiteral("notifyChange"));
    message.setArguments({
        0, // previous KGlobalSettings::PaletteChanged. This is now private API in khintsettings
        0 // unused in palette changed but needed for the DBus signature
    });
    QDBusConnection::sessionBus().send(message);
}
