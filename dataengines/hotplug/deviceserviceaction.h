/* This file is part of the KDE Project
   SPDX-FileCopyrightText: 2005 Jean-Remy Falleri <jr.falleri@laposte.net>
   SPDX-FileCopyrightText: 2005-2007 Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only
*/

#ifndef DEVICESERVICEACTION_H
#define DEVICESERVICEACTION_H

#include "deviceaction.h"

#include <kserviceaction.h>
#include <solid/predicate.h>

class DeviceServiceAction : public DeviceAction
{
public:
    DeviceServiceAction();
    QString id() const override;
    void execute(Solid::Device &device) override;

    void setService(const KServiceAction &service);
    KServiceAction service() const;

private:
    KServiceAction m_service;
};

#endif
