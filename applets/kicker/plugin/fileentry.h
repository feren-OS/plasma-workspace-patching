/***************************************************************************
 *   SPDX-FileCopyrightText: 2015 Eike Hein <hein@kde.org>                         *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#ifndef FILEENTRY_H
#define FILEENTRY_H

#include "abstractentry.h"

class KFileItem;

class FileEntry : public AbstractEntry
{
public:
    explicit FileEntry(AbstractModel *owner, const QUrl &url);
    ~FileEntry() override;

    EntryType type() const override
    {
        return RunnableType;
    }

    bool isValid() const override;

    QIcon icon() const override;
    QString name() const override;
    QString description() const override;

    QString id() const override;
    QUrl url() const override;

    bool hasActions() const override;
    QVariantList actions() const override;

    bool run(const QString &actionId = QString(), const QVariant &argument = QVariant()) override;

private:
    KFileItem *m_fileItem;
};

#endif
