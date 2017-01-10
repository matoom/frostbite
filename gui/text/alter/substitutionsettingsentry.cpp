#include "substitutionsettingsentry.h"

SubstitutionSettingsEntry::SubstitutionSettingsEntry() {
}

SubstitutionSettingsEntry::SubstitutionSettingsEntry(const int& id, const QString& pattern,
        const QString& substitute, const QStringList& targetList) {
    this->id = id;
    this->pattern = pattern;
    this->substitute = substitute;
    this->targetList = targetList;
}

const QString SubstitutionSettingsEntry::toString() {
    return "SubstitutionSettingsEntry:[ id => " + QString::number(this->id) +
            ", pattern => " + this->pattern +
            ", substitute => " + this->substitute + " ]";
}
