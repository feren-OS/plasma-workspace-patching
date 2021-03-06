/***************************************************************************
 *   Copyright (C) 2020 by Méven Car <meven.car@enioka.com>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA          *
 ***************************************************************************/
#ifndef AUTOSTARTMODEL_H
#define AUTOSTARTMODEL_H

#include <QAbstractListModel>
#include <QDir>

#include <KService>
#include <optional>

struct AutostartEntry;
class QQuickItem;

class AutostartModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AutostartModel(QObject *parent = nullptr);

    enum Roles {
        Name = Qt::DisplayRole,
        IconName = Qt::DecorationRole,
        Enabled = Qt::UserRole + 1,
        Source,
        FileName,
        OnlyInPlasma,
    };

    enum AutostartEntrySource {
        XdgAutoStart = 0,
        XdgScripts = 1,
        PlasmaShutdown = 2,
        PlasmaEnvScripts = 3,
    };
    Q_ENUM(AutostartEntrySource)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool reloadEntry(const QModelIndex &index, const QString &fileName);

    Q_INVOKABLE void removeEntry(int row);
    Q_INVOKABLE void editApplication(int row, QQuickItem *context);
    Q_INVOKABLE void addScript(const QUrl &url, AutostartEntrySource kind);
    Q_INVOKABLE void showApplicationDialog(QQuickItem *context);

    void load();

Q_SIGNALS:
    void error(const QString &message);

private:
    void addApplication(const KService::Ptr &service);
    void loadScriptsFromDir(const QString &subDir, AutostartEntrySource kind);
    void insertScriptEntry(int index, const QString &name, const QString &path, AutostartModel::AutostartEntrySource kind);
    static std::optional<AutostartEntry> loadDesktopEntry(const QString &fileName);

    QDir m_xdgConfigPath;
    QDir m_xdgAutoStartPath;
    QVector<AutostartEntry> m_entries;
};

struct AutostartEntry {
    QString name; // Human readable name or script file path. In case of symlinks the target file path
    AutostartModel::AutostartEntrySource source;
    bool enabled;
    QString fileName; // the file backing the entry
    bool onlyInPlasma;
    QString iconName;
};
Q_DECLARE_TYPEINFO(AutostartEntry, Q_MOVABLE_TYPE);

#endif // AUTOSTARTMODEL_H
