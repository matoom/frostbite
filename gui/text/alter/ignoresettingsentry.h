#ifndef IGNORESETTINGSENTRY_H
#define IGNORESETTINGSENTRY_H

#include <QList>

class IgnoreSettingsEntry {

public:
    IgnoreSettingsEntry();

    IgnoreSettingsEntry(const int& id, const QString& pattern, const QStringList& targetList);

    int id;
    QString pattern;
    QStringList targetList;
};

#endif // IGNORESETTINGSENTRY_H
