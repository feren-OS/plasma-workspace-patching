/*
 *   SPDX-FileCopyrightText: 2017 Marco MArtin <mart@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kcm.h"

#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

// Frameworks
#include <KAboutData>
#include <KLocalizedString>

#include <KPackage/Package>
#include <KPackage/PackageLoader>

#include "lookandfeelsettings.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const char version[] = "1.0";

    // About data
    KAboutData aboutData("plasma-apply-lookandfeel",
                         i18n("Global Theme Tool"),
                         version,
                         i18n("Command line tool to apply global theme packages for changing the look and feel."),
                         KAboutLicense::GPL,
                         i18n("Copyright 2017, Marco Martin"));
    aboutData.addAuthor(i18n("Marco Martin"), i18n("Maintainer"), QStringLiteral("mart@kde.org"));
    aboutData.setDesktopFileName("org.kde.plasma-apply-lookandfeel");
    KAboutData::setApplicationData(aboutData);

    const static auto _l = QStringLiteral("list");
    const static auto _a = QStringLiteral("apply");
    const static auto _r = QStringLiteral("resetLayout");

    QCommandLineOption _list = QCommandLineOption(QStringList() << QStringLiteral("l") << _l, i18n("List available global theme packages"));
    QCommandLineOption _apply = QCommandLineOption(QStringList() << QStringLiteral("a") << _a, i18n("Apply a global theme package. This can be the name of a package, or a full path to an installed package, at which point this tool will ensure it is a global theme package and then attempt to apply it"), i18n("packagename"));
    QCommandLineOption _resetLayout = QCommandLineOption(QStringList() << _r, i18n("Reset the Plasma Desktop layout"));

    QCommandLineParser parser;
    parser.addOption(_list);
    parser.addOption(_apply);
    parser.addOption(_resetLayout);
    aboutData.setupCommandLine(&parser);

    parser.process(app);
    aboutData.processCommandLine(&parser);

    if (!parser.isSet(_list) && !parser.isSet(_apply)) {
        parser.showHelp();
    }

    if (parser.isSet(_list)) {
        const QList<KPluginMetaData> pkgs = KPackage::PackageLoader::self()->listPackages("Plasma/LookAndFeel");

        for (const KPluginMetaData &data : pkgs) {
            std::cout << data.pluginId().toStdString() << std::endl;
        }

    } else if (parser.isSet(_apply)) {
        QString requestedTheme{parser.value(_apply)};
        if (!requestedTheme.endsWith(QStringLiteral("/"))) {
            // Just in case this isn't demarked as being a directory, let's just do that.
            // There would be other ways of doing this, but they all feel a bit convoluted
            // when this is all it really takes for our purposes.
            requestedTheme += QStringLiteral("/");
        }
        KPackage::Package p = KPackage::PackageLoader::self()->loadPackage("Plasma/LookAndFeel");
        p.setPath(requestedTheme);

        // can't use package.isValid as lnf packages always fallback, even when not existing
        if (p.metadata().pluginId() != requestedTheme) {
            if (!p.path().isEmpty() && p.path() == requestedTheme && QFile(p.path()).exists()) {
                std::cout << "Absolute path to theme passed in, set that" << std::endl;
                requestedTheme = p.metadata().pluginId();
            } else {
                std::cout << "Unable to find the theme named " << requestedTheme.toStdString() << std::endl;
                return 1;
            }
        }

        KCMLookandFeel *kcm = new KCMLookandFeel(nullptr, QVariantList());
        kcm->load();
        kcm->setResetDefaultLayout(parser.isSet(_resetLayout));
        kcm->lookAndFeelSettings()->setLookAndFeelPackage(requestedTheme);
        // Save manually as we aren't in an event loop
        kcm->lookAndFeelSettings()->save();
        kcm->save();
        delete kcm;
    }

    return 0;
}
