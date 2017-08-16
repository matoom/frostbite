#include "ignoresettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(IgnoreSettingsInstance, uniqueInstance)

IgnoreSettings* IgnoreSettings::getInstance() {
    if(uniqueInstance.exists()) {
        return uniqueInstance;
    } else {
        return new IgnoreSettingsInstance();
    }
}

IgnoreSettings::IgnoreSettings() {
    clientSettings = ClientSettings::getInstance();
    this->create();
}

void IgnoreSettings::reInit() {
    QWriteLocker locker(&lock);
    delete settings;
    this->create();
}

void IgnoreSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "ignores.ini", QSettings::IniFormat);
}

void IgnoreSettings::setSettings(QList<AlterSettingsEntry> entries) {
    QWriteLocker locker(&lock);
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
    QWriteLocker locker(&lock);
    int id = settings->value("ignore/size").toInt();

    settings->beginWriteArray("ignore");
    settings->setArrayIndex(id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void IgnoreSettings::setParameter(AlterSettingsEntry entry) {
    QWriteLocker locker(&lock);
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
    QList<AlterSettingsEntry> settingsCache; settingsCache = QList<AlterSettingsEntry>();
    this->loadSettings("ignore", settingsCache);
    return settingsCache;
}

void IgnoreSettings::loadSettings(QString group, QList<AlterSettingsEntry> &settingsList) {
    QReadLocker locker(&lock);
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
    delete settings;
}
