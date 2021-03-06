/*
   Copyright (c) 2014 Marco Martin <mart@kde.org>
   Copyright (c) 2014 Vishesh Handa <me@vhanda.in>
   Copyright (c) 2019 Cyril Rossi <cyril.rossi@enioka.com>
   Copyright (c) 2021 Benjamin Port <benjamin.port@enioka.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef _KCM_SEARCH_H
#define _KCM_SEARCH_H

#include <KConfig>
#include <KConfigGroup>
#include <QDir>

#include <KPackage/Package>
#include <KQuickAddons/ManagedConfigModule>

class QQuickItem;
class QStandardItemModel;
class DesktopLayoutSettings;
class DesktopLayoutData;

class KCMDesktopLayout : public KQuickAddons::ManagedConfigModule
{
    Q_OBJECT
    Q_PROPERTY(DesktopLayoutSettings *desktopLayoutSettings READ desktopLayoutSettings CONSTANT)
    Q_PROPERTY(QStandardItemModel *desktopLayoutModel READ desktopLayoutModel CONSTANT)

public:
    enum Roles {
        PluginNameRole = Qt::UserRole + 1,
        ScreenshotRole,
        FullScreenPreviewRole,
        DescriptionRole,
    };

    KCMDesktopLayout(QObject *parent, const QVariantList &args);
    ~KCMDesktopLayout() override;

    DesktopLayoutSettings *desktopLayoutSettings() const;
    QStandardItemModel *desktopLayoutModel() const;

    Q_INVOKABLE int pluginIndex(const QString &pluginName) const;

    // Setters of the various theme pieces
    void setFilesLayout(const QString &theme);
    void setLatteLayout(const QString &theme);
    void setBorderlessMaximised(const QString &value);
    void setWindowButtonsLayout(const QString &leftbtns, const QString &rightbtns);

    Q_INVOKABLE void reloadModel();

    bool isSaveNeeded() const override;

public Q_SLOTS:
    void load() override;
    void save() override;
    void defaults() override;

private:
    // List only packages which provide at least one of the components
    QList<KPackage::Package> availablePackages(const QStringList &components);
    void loadModel();
    QDir cursorThemeDir(const QString &theme, const int depth);
    const QStringList cursorSearchPaths();

    void revertKeyIfNeeded(KConfigGroup &group, KConfigGroup &home, KConfigGroup &defaults);

    void writeNewDefaults(const QString &filename,
                          const QString &group,
                          const QString &key,
                          const QString &value,
                          KConfig::WriteConfigFlags writeFlags = KConfig::Normal);
    void writeNewDefaults(KConfig &config,
                          KConfig &configDefault,
                          const QString &group,
                          const QString &key,
                          const QString &value,
                          KConfig::WriteConfigFlags writeFlags = KConfig::Normal);
    void
    writeNewDefaults(KConfigGroup &cg, KConfigGroup &cgd, const QString &key, const QString &value, KConfig::WriteConfigFlags writeFlags = KConfig::Normal);
    static KConfig configDefaults(const QString &filename);

    DesktopLayoutData *m_data;
    QStandardItemModel *m_model;
    KPackage::Package m_package;
    QStringList m_cursorSearchPaths;

    KConfig m_config;
    KConfigGroup m_configGroup;
};

#endif
