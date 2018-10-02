#include "ignoresettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(IgnoreSettingsInstance, uniqueInstance)

IgnoreSettings* IgnoreSettings::getInstance() {
    return uniqueInstance;
}

IgnoreSettings::IgnoreSettings() {
    clientSettings = ClientSettings::getInstance();
    this->create();
}

void IgnoreSettings::reInit() {
    QMutexLocker locker(&m_mutex);
    delete settings;
    this->create();
}

void IgnoreSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "ignores.ini", QSettings::IniFormat);
}

void IgnoreSettings::setSettings(QList<AlterSettingsEntry> entries) {
    QMutexLocker locker(&m_mutex);
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

void IgnoreSettings::setEnabled(bool enabled) {
    QMutexLocker locker(&m_mutex);
    settings->setValue("settings/enabled", enabled);
}

bool IgnoreSettings::getEnabled() {
    QMutexLocker locker(&m_mutex);
    return settings->value("settings/enabled", true).toBool();
}

void IgnoreSettings::addParameter(AlterSettingsEntry entry) {
    QMutexLocker locker(&m_mutex);
    int id = settings->value("ignore/size").toInt();

    settings->beginWriteArray("ignore");
    settings->setArrayIndex(id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void IgnoreSettings::setParameter(AlterSettingsEntry entry) {
    QMutexLocker locker(&m_mutex);
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
    QMutexLocker locker(&m_mutex);
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
