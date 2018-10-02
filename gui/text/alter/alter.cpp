#include "alter.h"

Alter::Alter(QObject *parent) : QObject(parent) {
    ignoreSettings = IgnoreSettings::getInstance();
    ignoreList = ignoreSettings->getIgnores();
    substituteSettings = SubstitutionSettings::getInstance();
    subsList = substituteSettings->getSubstitutions();
    ignoreEnabled = ignoreSettings->getEnabled();
}

void Alter::reloadSettings() {
    ignoreSettings->reInit();
    ignoreList = ignoreSettings->getIgnores();
    substituteSettings->reInit();
    subsList = substituteSettings->getSubstitutions();
    ignoreEnabled = ignoreSettings->getEnabled();
}

QString Alter::substitute(QString text, QString window) {
    if(!text.isEmpty()) {        
        for(AlterSettingsEntry entry : subsList) {
            if(!entry.enabled || entry.pattern.isEmpty()) continue;
            if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;
            text.replace(QRegularExpression(entry.pattern + "(?=[^>]*(<|$))"), entry.substitute);
        }
    }
    return text;
}

bool Alter::ignore(QString text, QString window) {
    if(!ignoreEnabled) return false;
    for(AlterSettingsEntry entry : ignoreList) {
        if(!entry.enabled || entry.pattern.isEmpty()) continue;
        if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;
        if (QRegularExpression(entry.pattern + "(?=[^>]*(<|$))").match(text).hasMatch()) {
           return true;
        }
    }
    return false;
}

Alter::~Alter() {
}
