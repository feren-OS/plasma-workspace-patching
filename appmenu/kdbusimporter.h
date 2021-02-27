/*
  This file is part of the KDE project.

  SPDX-FileCopyrightText: 2011 Lionel Chauvin <megabigbug@yahoo.fr>
  SPDX-FileCopyrightText: 2011, 2012 Cédric Bellegarde <gnumdk@gmail.com>

  SPDX-License-Identifier: MIT
*/

#ifndef KDBUSIMPORTER_H
#define KDBUSIMPORTER_H

#include <dbusmenuimporter.h>

#include "verticalmenu.h"
#include <QIcon>

class KDBusMenuImporter : public DBusMenuImporter
{
public:
    KDBusMenuImporter(const QString &service, const QString &path, QObject *parent)
        : DBusMenuImporter(service, path, parent)
    {
    }

protected:
    QIcon iconForName(const QString &name) override
    {
        return QIcon::fromTheme(name);
    }

    QMenu *createMenu(QWidget *parent) override
    {
        return new VerticalMenu(parent);
    }
};

#endif // KDBUSIMPORTER_H
