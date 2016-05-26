#include "highlightsettings.h"

HighlightSettings::HighlightSettings() {
    this->init();
}

void HighlightSettings::init() {
    QString path = ClientSettings::Instance()->profilePath();
    settings = new QSettings(path + "highlights.ini", QSettings::IniFormat);
    settingsList = NULL;
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

void HighlightSettings::loadSettings(QString group) {
    settingsList = new QList<HighlightSettingsEntry>();

    int size = settings->beginReadArray(group);

    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);

        settingsList->append(HighlightSettingsEntry((const int&)i,
            (const QString&)settings->value("value").toString(),
            (const QString&)settings->value("group").toString(),
            (const QColor&)settings->value("color").value<QColor>(),
            (const bool&)settings->value("alert").toBool(),
            (const QString&)settings->value("alertValue").toString(),
            (const bool&)settings->value("timer").toBool(),
            (const int&)settings->value("timerValue").toInt(),
            (const QString&)settings->value("timerAction").toString(),
            (const QBitArray&)settings->value("options").value<QBitArray>()));
    }
    settings->endArray();
}

QList<HighlightSettingsEntry>* HighlightSettings::getSettings(QString group) {       
    if(!settingsList) {
        this->loadSettings(group);
    }
    return settingsList;
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
    delete settingsList;
}
