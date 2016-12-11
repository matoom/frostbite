#include "highlightsettings.h"

HighlightSettings::HighlightSettings() {
    initSettings = true;
    settingsCache = new QList<HighlightSettingsEntry>();
    this->create();
}

void HighlightSettings::init() {
    initSettings = true;
}

void HighlightSettings::create() {
    QString path = ClientSettings::Instance()->profilePath();
    settings = new QSettings(path + "highlights.ini", QSettings::IniFormat);
}

void HighlightSettings::setSingleParameter(QString name, QVariant value) {
    settings->setValue(name, value);
}

QVariant HighlightSettings::getSingleParameter(QString name, QVariant defaultValue) {
    return settings->value(name, defaultValue);
}

void HighlightSettings::addParameter(QString group, HighlightSettingsEntry entry) {
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

void HighlightSettings::loadSettings(QString group, QList<HighlightSettingsEntry>* settingsList) {
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

// TODO: profile cache/remove?
QList<HighlightSettingsEntry>* HighlightSettings::getTextHighlights() {
    if(initSettings) {
        settings->deleteLater();
        this->create();
        settingsCache->clear();
        this->loadSettings("TextHighlight", settingsCache);
        initSettings = false;
    }
    return settingsCache;
}

void HighlightSettings::setSettings(QString group, QList<HighlightSettingsEntry>* settingsList) {
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
}
