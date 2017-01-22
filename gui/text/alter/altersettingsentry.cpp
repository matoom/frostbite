#include "altersettingsentry.h"

AlterSettingsEntry::AlterSettingsEntry() {

}

AlterSettingsEntry::AlterSettingsEntry(const int& id, const bool& enabled, const QString& pattern,
        const QStringList& targetList) {
    this->id = id;
    this->enabled = enabled;
    this->pattern = pattern;    

    this->targetList = targetList;
}

AlterSettingsEntry::AlterSettingsEntry(const int& id, const bool& enabled, const QString& pattern,
        const QString& substitute, const QStringList& targetList) {
    this->id = id;
    this->enabled = enabled;
    this->pattern = pattern;
    this->substitute = substitute;
    this->targetList = targetList;
}

const QString AlterSettingsEntry::toString() {
    return "RegexpSettingsEntry:[ id => " + QString::number(this->id) +
            ", pattern => " + this->pattern +
            ", substitute => " + this->substitute + " ]";
}
