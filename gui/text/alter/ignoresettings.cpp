#include "ignoresettings.h"

IgnoreSettings::IgnoreSettings() {
    clientSettings = new ClientSettings();

    initSettings = true;
    settingsCache = QList<AlterSettingsEntry>();
    this->create();
}

void IgnoreSettings::init() {
    initSettings = true;
}

void IgnoreSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "ignores.ini", QSettings::IniFormat);
}

void IgnoreSettings::setSettings(QList<AlterSettingsEntry> entries) {
    settings->remove("ignore");
    settings->beginWriteArray("ignore");

    for (int i = 0; i < entries.size(); ++i) {
        AlterSettingsEntry entry = entries.at(i);

        settings->setArrayIndex(i);
        settings->setValue("enabled", entry.enabled);
        settings->setValue("pattern", entry.pattern);
        settings->setValue("target", entry.targetList);
    }
    settings->endArray();
}

void IgnoreSettings::addParameter(AlterSettingsEntry entry) {
    int id = settings->value("ignore/size").toInt();

    settings->beginWriteArray("ignore");
    settings->setArrayIndex(id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void IgnoreSettings::setParameter(AlterSettingsEntry entry) {
    int size = settings->value("ignore/size").toInt();

    settings->beginWriteArray("ignore");
    settings->setArrayIndex(entry.id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("target", entry.targetList);
    settings->endArray();

    settings->setValue("ignore/size", size);
}

QList<AlterSettingsEntry> IgnoreSettings::getIgnores() {
    if(initSettings) {
        settings->deleteLater();
        this->create();
        settingsCache.clear();
        this->loadSettings("ignore", settingsCache);
        initSettings = false;
    }
    return settingsCache;
}

void IgnoreSettings::loadSettings(QString group, QList<AlterSettingsEntry> &settingsList) {
    int size = settings->beginReadArray(group);
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        settingsList.append(AlterSettingsEntry((const int&)i,
                (const bool&)settings->value("enabled", "").toBool(),
                (const QString&)settings->value("pattern", "").toString(),
                (const QStringList&)settings->value("target", QStringList()).value<QStringList>()));
    }
    settings->endArray();
}

IgnoreSettings::~IgnoreSettings() {
    delete clientSettings;
    delete settings;
}
