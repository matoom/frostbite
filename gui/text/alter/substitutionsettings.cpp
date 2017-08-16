#include "substitutionsettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(SubstitutionSettingsInstance, uniqueInstance)

SubstitutionSettings* SubstitutionSettings::getInstance() {
    if(uniqueInstance.exists()) {
        return uniqueInstance;
    } else {
        return new SubstitutionSettingsInstance();
    }
}

SubstitutionSettings::SubstitutionSettings() {
    clientSettings = ClientSettings::getInstance();
    this->create();
}

void SubstitutionSettings::reInit() {
    QWriteLocker locker(&lock);
    delete settings;
    this->create();
}

void SubstitutionSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "substitutes.ini", QSettings::IniFormat);
}

void SubstitutionSettings::setSettings(QList<AlterSettingsEntry> entries) {
    QWriteLocker locker(&lock);
    settings->remove("substitution");
    settings->beginWriteArray("substitution");

    for (int i = 0; i < entries.size(); ++i) {
        AlterSettingsEntry entry = entries.at(i);

        settings->setArrayIndex(i);
        settings->setValue("enabled", entry.enabled);
        settings->setValue("pattern", entry.pattern);
        settings->setValue("substitute", entry.substitute);
        settings->setValue("target", entry.targetList);
    }
    settings->endArray();
}

void SubstitutionSettings::addParameter(AlterSettingsEntry entry) {
    QWriteLocker locker(&lock);
    int id = settings->value("substitution/size").toInt();

    settings->beginWriteArray("substitution");
    settings->setArrayIndex(id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("substitute", entry.substitute);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void SubstitutionSettings::setParameter(AlterSettingsEntry entry) {
    QWriteLocker locker(&lock);
    int size = settings->value("substitution/size").toInt();

    settings->beginWriteArray("substitution");
    settings->setArrayIndex(entry.id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("substitute", entry.substitute);
    settings->setValue("target", entry.targetList);
    settings->endArray();

    settings->setValue("substitution/size", size);
}

QList<AlterSettingsEntry> SubstitutionSettings::getSubstitutions() {
    QList<AlterSettingsEntry> settingsCache = QList<AlterSettingsEntry>();
    this->loadSettings("substitution", settingsCache);
    return settingsCache;
}

void SubstitutionSettings::loadSettings(QString group, QList<AlterSettingsEntry> &settingsList) {
    QReadLocker locker(&lock);
    int size = settings->beginReadArray(group);
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        settingsList.append(AlterSettingsEntry((const int&)i,
                (const bool&)settings->value("enabled", "").toBool(),
                (const QString&)settings->value("pattern", "").toString(),
                (const QString&)settings->value("substitute", "").toString(),
                (const QStringList&)settings->value("target", QStringList()).value<QStringList>()));
    }
    settings->endArray();
}

SubstitutionSettings::~SubstitutionSettings() {
    delete settings;
}
