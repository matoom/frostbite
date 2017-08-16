#include "alter.h"

Alter::Alter(QObject *parent) : QObject(parent) {
    ignoreSettings = IgnoreSettings::getInstance();
    substituteSettings = SubstitutionSettings::getInstance();
}

void Alter::reloadSettings() {
    ignoreSettings->reInit();
    substituteSettings->reInit();
}

QString Alter::substitute(QString text, QString window) {
    if(!text.isEmpty()) {
        QList<AlterSettingsEntry> alterList = substituteSettings->getSubstitutions();
        for(AlterSettingsEntry entry : alterList) {
            if(!entry.enabled || entry.pattern.isEmpty()) continue;
            if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;
            text.replace(QRegularExpression(entry.pattern + "(?=[^>]*(<|$))"), entry.substitute);
        }
    }
    return text;
}

bool Alter::ignore(QString text, QString window) {
    QList<AlterSettingsEntry> ignoreList = ignoreSettings->getIgnores();
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
    delete ignoreSettings;
}
