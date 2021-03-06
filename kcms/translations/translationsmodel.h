/*
 *  Copyright (C) 2014 John Layt <john@layt.net>
 *  Copyright (C) 2018 Eike Hein <hein@kde.org>
 *  Copyright (C) 2021 Harald Sitter <sitter@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef TRANSLATIONSMODEL_H
#define TRANSLATIONSMODEL_H

#include <QAbstractListModel>

#include <KConfigGroup>

#include <QSet>

class TranslationsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AdditionalRoles {
        LanguageCode = Qt::UserRole + 1,
        IsMissing,
        IsIncomplete,
        IsInstalling,
    };
    Q_ENUM(AdditionalRoles)

    explicit TranslationsModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QString languageCodeToName(const QString &languageCode) const;

    static QStringList m_languages;

    static QSet<QString> m_installedLanguages;
};

class SelectedTranslationsModel : public TranslationsModel
{
    Q_OBJECT

    Q_PROPERTY(QStringList selectedLanguages READ selectedLanguages WRITE setSelectedLanguages NOTIFY selectedLanguagesChanged)
    Q_PROPERTY(QStringList missingLanguages READ missingLanguages NOTIFY missingLanguagesChanged)

public:
    using TranslationsModel::TranslationsModel;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QStringList selectedLanguages() const;
    void setSelectedLanguages(const QStringList &languages);

    QStringList missingLanguages() const;
    Q_INVOKABLE void completeLanguage(int index);

    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove(const QString &languageCode);

Q_SIGNALS:
    void selectedLanguagesChanged(const QStringList &languages) const;
    void missingLanguagesChanged() const;

private:
    void reloadCompleteness(const QString &languageCode);

    QStringList m_selectedLanguages;
    QStringList m_missingLanguages;

    QHash<QString, QStringList> m_incompleteLanguagesWithPackages;
    QStringList m_installingLanguages;
};

class AvailableTranslationsModel : public TranslationsModel
{
    Q_OBJECT

public:
    using TranslationsModel::TranslationsModel;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QString langCodeAt(int row);

public Q_SLOTS:
    void setSelectedLanguages(const QStringList &languages);

private:
    QStringList m_availableLanguages;
};

#endif
