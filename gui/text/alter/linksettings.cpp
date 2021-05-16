#include "linksettings.h"

#include "clientsettings.h"

#include <QGlobalStatic>

Q_GLOBAL_STATIC(LinkSettingsInstance, uniqueInstance)

LinkSettings* LinkSettings::getInstance() {
    return uniqueInstance;
}

LinkSettings::LinkSettings() {
    clientSettings = ClientSettings::getInstance();
    this->create();
}

void LinkSettings::reInit() {
    QMutexLocker locker(&m_mutex);
    delete settings;
    this->create();
}

void LinkSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "links.ini", QSettings::IniFormat);
}

void LinkSettings::setSettings(QList<AlterSettingsEntry> entries) {
    QMutexLocker locker(&m_mutex);
    settings->remove("links");
    settings->beginWriteArray("links");

    for (int i = 0; i < entries.size(); ++i) {
        AlterSettingsEntry entry = entries.at(i);

        settings->setArrayIndex(i);
        settings->setValue("enabled", entry.enabled);
        settings->setValue("pattern", entry.pattern);
        settings->setValue("link", entry.value);
        settings->setValue("target", entry.targetList);
    }
    settings->endArray();
}

void LinkSettings::setEnabled(bool enabled) {
    QMutexLocker locker(&m_mutex);
    settings->setValue("settings/enabled", enabled);
}

bool LinkSettings::getEnabled() {
    QMutexLocker locker(&m_mutex);
    return settings->value("settings/enabled", true).toBool();
}

void LinkSettings::addParameter(AlterSettingsEntry entry) {
    QMutexLocker locker(&m_mutex);
    int id = settings->value("links/size").toInt();

    settings->beginWriteArray("links");
    settings->setArrayIndex(id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("link", entry.value);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void LinkSettings::setParameter(AlterSettingsEntry entry) {
    QMutexLocker locker(&m_mutex);
    int size = settings->value("links/size").toInt();

    settings->beginWriteArray("links");
    settings->setArrayIndex(entry.id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("link", entry.value);
    settings->setValue("target", entry.targetList);
    settings->endArray();

    settings->setValue("links/size", size);
}

QList<AlterSettingsEntry> LinkSettings::getLinks() {
    QList<AlterSettingsEntry> settingsCache = QList<AlterSettingsEntry>();
    this->loadSettings("links", settingsCache);
    return settingsCache;
}

void LinkSettings::loadSettings(QString group, QList<AlterSettingsEntry> &settingsList) {
    QMutexLocker locker(&m_mutex);
    int size = settings->beginReadArray(group);
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        settingsList.append(AlterSettingsEntry((const int&)i,
                (const bool&)settings->value("enabled", "").toBool(),
                (const QString&)settings->value("pattern", "").toString(),
                (const QString&)settings->value("link", "").toString(),
                (const QStringList&)settings->value("target", QStringList()).value<QStringList>()));
    }
    settings->endArray();
}

LinkSettings::~LinkSettings() {
    delete settings;
}
