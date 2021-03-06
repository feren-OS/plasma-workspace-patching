/*
 * Copyright 2019 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QSortFilterProxyModel>

#include "notifications.h"

namespace NotificationManager
{
class NotificationSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit NotificationSortProxyModel(QObject *parent = nullptr);
    ~NotificationSortProxyModel() override;

    Notifications::SortMode sortMode() const;
    void setSortMode(Notifications::SortMode);

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder sortOrder);

Q_SIGNALS:
    void sortModeChanged();
    void sortOrderChanged();

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    Notifications::SortMode m_sortMode = Notifications::SortByDate;
    Qt::SortOrder m_sortOrder = Qt::DescendingOrder;
};

} // namespace NotificationManager
