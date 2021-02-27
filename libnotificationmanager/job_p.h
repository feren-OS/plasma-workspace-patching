/*
 * SPDX-FileCopyrightText: 2019 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QDBusContext>
#include <QDBusObjectPath>
#include <QDBusVariant>
#include <QDateTime>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QUrl>

#include "job.h"
#include "notifications.h"

class QTimer;
class KFilePlacesModel;

namespace NotificationManager
{
class JobPrivate : public QObject, protected QDBusContext
{
    Q_OBJECT

public:
    JobPrivate(uint id, QObject *parent);
    ~JobPrivate() override;

    QDBusObjectPath objectPath() const;
    QUrl descriptionUrl() const;
    QString text() const;

    void kill();

    // DBus
    // JobViewV1
    void terminate(const QString &errorMessage);
    void setSuspended(bool suspended);
    void setTotalAmount(quint64 amount, const QString &unit);
    void setProcessedAmount(quint64 amount, const QString &unit);
    void setPercent(uint percent);
    void setSpeed(quint64 bytesPerSecond);
    void setInfoMessage(const QString &infoMessage);
    bool setDescriptionField(uint number, const QString &name, const QString &value);
    void clearDescriptionField(uint number);
    void setDestUrl(const QDBusVariant &urlVariant);
    void setError(uint errorCode);

    // JobViewV2
    void terminate(uint errorCode, const QString &errorMessage, const QVariantMap &hints);
    void update(const QVariantMap &properties);

signals:
    void closed();

    void infoMessageChanged();

    // DBus
    // V1 and V2
    void suspendRequested();
    void resumeRequested();
    void cancelRequested();
    // V2
    void updateRequested();

private:
    friend class Job;

    template<typename T> bool updateField(const T &newValue, T &target, void (Job::*changeSignal)())
    {
        if (target != newValue) {
            target = newValue;
            emit((static_cast<Job *>(parent()))->*changeSignal)();
            return true;
        }
        return false;
    }

    template<typename T> bool updateFieldFromProperties(const QVariantMap &properties, const QString &keyName, T &target, void (Job::*changeSignal)())
    {
        auto it = properties.find(keyName);
        if (it == properties.end()) {
            return false;
        }

        return updateField(it->value<T>(), target, changeSignal);
    }

    static QSharedPointer<KFilePlacesModel> createPlacesModel();

    static QUrl localFileOrUrl(const QString &stringUrl);

    QUrl destUrl() const;
    QString prettyUrl(const QUrl &url) const;
    void updateHasDetails();

    void finish();

    QTimer *m_killTimer = nullptr;

    uint m_id = 0;
    QDBusObjectPath m_objectPath;

    QDateTime m_created;
    QDateTime m_updated;

    QString m_summary;
    QString m_infoMessage;

    QString m_desktopEntry;
    QString m_applicationName;
    QString m_applicationIconName;

    Notifications::JobState m_state = Notifications::JobStateRunning;
    int m_percentage = 0;
    int m_error = 0;
    QString m_errorText;
    bool m_suspendable = false;
    bool m_killable = false;

    QUrl m_destUrl;

    qulonglong m_speed = 0;

    qulonglong m_processedBytes = 0;
    qulonglong m_processedFiles = 0;
    qulonglong m_processedDirectories = 0;
    qulonglong m_processedItems = 0;

    qulonglong m_totalBytes = 0;
    qulonglong m_totalFiles = 0;
    qulonglong m_totalDirectories = 0;
    qulonglong m_totalItems = 0;

    QString m_descriptionLabel1;
    QString m_descriptionValue1;

    QString m_descriptionLabel2;
    QString m_descriptionValue2;

    bool m_hasDetails = false;

    bool m_expired = false;
    bool m_dismissed = false;

    mutable QSharedPointer<KFilePlacesModel> m_placesModel;
};

} // namespace NotificationManager
