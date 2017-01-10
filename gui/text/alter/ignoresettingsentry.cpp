#include "ignoresettingsentry.h"

IgnoreSettingsEntry::IgnoreSettingsEntry() {
}

IgnoreSettingsEntry::IgnoreSettingsEntry(const int& id, const QString& pattern,
        const QStringList& targetList) {

    this->id = id;
    this->pattern = pattern;
    this->targetList = targetList;
}
