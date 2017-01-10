#ifndef SUBSTITUTIONSETTINGSENTRY_H
#define SUBSTITUTIONSETTINGSENTRY_H

#include <QList>

class SubstitutionSettingsEntry {

public:
    SubstitutionSettingsEntry();

    SubstitutionSettingsEntry(const int& id, const QString& pattern, const QString& substitute, const QStringList& targetList);

    const QString toString();

    int id;
    QString pattern;
    QString substitute;
    QStringList targetList;
};

typedef QList<SubstitutionSettingsEntry> SubstitutionSettingsEntryList;

#endif // SUBSTITUTIONSETTINGSENTRY_H
