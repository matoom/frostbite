#include "alter.h"

Alter::Alter(QObject *parent) : QObject(parent) {
    ignoreSettings = new IgnoreSettings();
    substituteSettings = new SubstitutionSettings();
}

void Alter::reloadSettings() {
    delete ignoreSettings;
    ignoreSettings = new IgnoreSettings();
    delete substituteSettings;
    substituteSettings = new SubstitutionSettings();
}

QString Alter::substitute(QString text, QString window) {
    if(!text.isEmpty()) {
        QList<AlterSettingsEntry> alterList = substituteSettings->getSubstitutions();
        for(AlterSettingsEntry entry : alterList) {
            if(entry.pattern.isEmpty()) continue;
            if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;

            TextUtils::plainToHtml(entry.pattern);
            TextUtils::plainToHtml(entry.substitute);

            text.replace(QRegExp(entry.pattern + "(?=[^>]*(<|$))"), entry.substitute);
        }
    }
    return text;
}

bool Alter::ignore(QString text, QString window) {
    QList<AlterSettingsEntry> ignoreList = ignoreSettings->getIgnores();
    for(AlterSettingsEntry entry : ignoreList) {
        if(entry.pattern.isEmpty()) continue;
        if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;

        TextUtils::plainToHtml(entry.pattern);
        if(QRegExp(entry.pattern + "(?=[^>]*(<|$))").indexIn(text) != -1) {
            return true;
        }
    }
    return false;
}
