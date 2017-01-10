#ifndef ALTERSETTINGSENTRY_H
#define ALTERSETTINGSENTRY_H

#include <QList>

class AlterSettingsEntry {

public:
    AlterSettingsEntry();

    AlterSettingsEntry(const int& id, const QString& pattern, const QStringList& targetList);

    AlterSettingsEntry(const int& id, const QString& pattern, const QString& substitute, const QStringList& targetList);

    const QString toString();

    int id;
    QString pattern;
    QString substitute;
    QStringList targetList;
};

typedef QList<AlterSettingsEntry> alterSettingsEntryList;

#endif // ALTERSETTINGSENTRY_H
