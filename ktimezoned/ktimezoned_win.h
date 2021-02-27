/*
   This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2009 Till Adam <adam@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KTIMEZONED_H
#define KTIMEZONED_H

#include "ktimezonedbase.h"

class RegistryWatcherThread;

class KTimeZoned : public KTimeZonedBase
{
    Q_OBJECT
    friend class RegistryWatcherThread;

public:
    KTimeZoned(QObject *parent, const QList<QVariant> &);
    ~KTimeZoned();

private:
    /** reimp */
    void init(bool);
    void updateLocalZone();

    RegistryWatcherThread *mRegistryWatcherThread; // thread that watches the timezone registry key
};

#endif
