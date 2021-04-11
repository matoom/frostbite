#include "alter.h"

#include "text/alter/substitutionsettings.h"
#include "text/alter/ignoresettings.h"
#include "text/alter/linksettings.h"
#include "hyperlinkservice.h"
#include "globaldefines.h"

Alter::Alter(QObject *parent) : QObject(parent) {
    ignoreSettings = IgnoreSettings::getInstance();
    ignoreList = ignoreSettings->getIgnores();
    ignoreEnabled = ignoreSettings->getEnabled();

    substituteSettings = SubstitutionSettings::getInstance();
    subsList = substituteSettings->getSubstitutions();

    linkSettings = LinkSettings::getInstance();
    linkList = linkSettings->getLinks();
}

void Alter::reloadSettings() {
    ignoreSettings->reInit();
    ignoreList = ignoreSettings->getIgnores();
    substituteSettings->reInit();
    subsList = substituteSettings->getSubstitutions();
    ignoreEnabled = ignoreSettings->getEnabled();
    linkSettings->reInit();
    linkList = linkSettings->getLinks();
}

QString Alter::substitute(QString text, QString window) {
    if(!text.isEmpty()) {        
        for(AlterSettingsEntry entry : subsList) {
            if(!entry.enabled || entry.pattern.isEmpty()) continue;
            if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;
            text.replace(QRegularExpression(entry.pattern + "(?=[^>]*(<|$))"), entry.value);
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

QString Alter::addLink(QString text, QString window) {
    if(!text.isEmpty()) {
        for(AlterSettingsEntry entry : linkList) {
            if(!entry.enabled || entry.pattern.isEmpty() || entry.value.isEmpty()) continue;
            if(!entry.targetList.empty() && !entry.targetList.contains(window)) continue;
            HyperlinkService::addLink(text, entry.pattern, entry.value);
        }
    }
    return text;
}

Alter::~Alter() {
}
