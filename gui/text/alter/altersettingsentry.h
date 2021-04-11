#ifndef ALTERSETTINGSENTRY_H
#define ALTERSETTINGSENTRY_H

#include <QList>

class AlterSettingsEntry {

public:
    AlterSettingsEntry();
    AlterSettingsEntry(const int& id, const bool& enabled, const QString& pattern, const QStringList& targetList);
    AlterSettingsEntry(const int& id, const bool& enabled, const QString& pattern, const QString& value, const QStringList& targetList);

    const QString toString();

    int id;
    bool enabled;
    QString pattern;
    QString value;
    QStringList targetList;
};

typedef QList<AlterSettingsEntry> alterSettingsEntryList;

#endif // ALTERSETTINGSENTRY_H
