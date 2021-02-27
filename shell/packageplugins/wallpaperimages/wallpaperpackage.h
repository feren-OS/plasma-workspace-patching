/*
 *   SPDX-FileCopyrightText: 2013 Marco Martin <mart@kde.org>

 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef WALLPAPERPACKAGE_H
#define WALLPAPERPACKAGE_H

#include <KPackage/PackageStructure>

class WallpaperPackage : public KPackage::PackageStructure
{
    Q_OBJECT

public:
    explicit WallpaperPackage(QObject *parent = nullptr, const QVariantList &args = QVariantList());

    void initPackage(KPackage::Package *package) override;
    void pathChanged(KPackage::Package *package) override;
};

#endif
