#include "highlightsettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(HighlightSettingsInstance, uniqueInstance)

HighlightSettings* HighlightSettings::getInstance() {
    if(uniqueInstance.exists()) {
        return uniqueInstance;
    } else {
        return new HighlightSettingsInstance();
    }
}

HighlightSettings::HighlightSettings() {
    clientSettings = ClientSettings::getInstance();
    this->create();
}

void HighlightSettings::reInit() {
    QWriteLocker locker(&lock);
    delete settings;
    this->create();
}

void HighlightSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "highlights.ini", QSettings::IniFormat);
}

void HighlightSettings::setSingleParameter(QString name, QVariant value) {
    QWriteLocker locker(&lock);
    settings->setValue(name, value);
}

QVariant HighlightSettings::getSingleParameter(QString name, QVariant defaultValue) {
    QWriteLocker locker(&lock);
    return settings->value(name, defaultValue);
}

void HighlightSettings::addParameter(QString group, HighlightSettingsEntry entry) {
    QWriteLocker locker(&lock);
    int id = settings->value(group + "/size").toInt();

    settings->beginWriteArray(group);
    settings->setArrayIndex(id);
    settings->setValue("value", entry.value);
    settings->setValue("group", entry.group);
    settings->setValue("color", entry.color);
    settings->setValue("alert", entry.alert);
    settings->setValue("alertValue", entry.alertValue);
    settings->setValue("timer", entry.timer);
    settings->setValue("timerValue", entry.timerValue);
    settings->setValue("timerAction", entry.timerAction);
    settings->setValue("options", entry.options);
    settings->endArray();
}

void HighlightSettings::setParameter(QString group, HighlightSettingsEntry entry) {
    QWriteLocker locker(&lock);
    int size = settings->value(group + "/size").toInt();

    settings->beginWriteArray(group);
    settings->setArrayIndex(entry.id);
    settings->setValue("value", entry.value);
    settings->setValue("group", entry.group);
    settings->setValue("color", entry.color);
    settings->setValue("alert", entry.alert);
    settings->setValue("alertValue", entry.alertValue);
    settings->setValue("timer", entry.timer);
    settings->setValue("timerValue", entry.timerValue);
    settings->setValue("timerAction", entry.timerAction);
    settings->setValue("options", entry.options);
    settings->endArray();

    settings->setValue(group + "/size", size);
}

// TODO: profile cache/remove?
QList<HighlightSettingsEntry>* HighlightSettings::getTextHighlights() {
    QList<HighlightSettingsEntry>* settingsCache = new QList<HighlightSettingsEntry>();
    this->loadSettings("TextHighlight", settingsCache);
    return settingsCache;
}

void HighlightSettings::loadSettings(QString group, QList<HighlightSettingsEntry>* settingsList) {
    QReadLocker locker(&lock);
    int size = settings->beginReadArray(group);

    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);

        settingsList->append(HighlightSettingsEntry((const int&)i,
                (const QString&)settings->value("value", "").toString(),
                (const QString&)settings->value("group", "").toString(),
                (const QColor&)settings->value("color", QColor()).value<QColor>(),
                (const bool&)settings->value("alert", false).toBool(),
                (const QString&)settings->value("alertValue", "").toString(),
                (const bool&)settings->value("timer", false).toBool(),
                (const int&)settings->value("timerValue", 0).toInt(),
                (const QString&)settings->value("timerAction", "").toString(),
                (const QBitArray&)settings->value("options", QBitArray(3)).value<QBitArray>()));
    }
    settings->endArray();
}

void HighlightSettings::setSettings(QString group, QList<HighlightSettingsEntry>* settingsList) {
    QWriteLocker locker(&lock);
    settings->remove(group);
    settings->beginWriteArray(group);

    for (int i = 0; i < settingsList->size(); ++i) {
        HighlightSettingsEntry entry = settingsList->at(i);

        settings->setArrayIndex(i);
        settings->setValue("value", entry.value);
        settings->setValue("group", entry.group);
        settings->setValue("color", entry.color);
        settings->setValue("alert", entry.alert);
        settings->setValue("alertValue", entry.alertValue);
        settings->setValue("timer", entry.timer);
        settings->setValue("timerValue", entry.timerValue);
        settings->setValue("timerAction", entry.timerAction);
        settings->setValue("options", entry.options);
    }
    settings->endArray();
}

HighlightSettings::~HighlightSettings() {
    delete settings;
    delete settingsCache;
    delete uniqueInstance;
}
