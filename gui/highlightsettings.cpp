#include "highlightsettings.h"

HighlightSettings::HighlightSettings() : QSettings("highlights.ini", QSettings::IniFormat) {
    //m_sSettingsFile = QApplication::applicationDirPath().left(1) + ":/demosettings.ini";
}

void HighlightSettings::setSingleParameter(QString name, QVariant value) {
    setValue(name, value);
}

QVariant HighlightSettings::getSingleParameter(QString name, QVariant defaultValue) {
    return value(name, defaultValue);
}

void HighlightSettings::addParameter(QString group, HighlightSettingsEntry entry) {
    int id = value(group + "/size").toInt();

    beginWriteArray(group);
    setArrayIndex(id);
    setValue("value", entry.value);
    setValue("group", entry.group);
    setValue("color", entry.color);
    setValue("alert", entry.alert);
    setValue("alertValue", entry.alertValue);
    setValue("timer", entry.timer);
    setValue("timerValue", entry.timerValue);
    setValue("timerAction", entry.timerAction);
    setValue("options", entry.options);
    endArray();
}

void HighlightSettings::setParameter(QString group, HighlightSettingsEntry entry) {
    int size = value(group + "/size").toInt();

    beginWriteArray(group);
    setArrayIndex(entry.id);
    setValue("value", entry.value);
    setValue("group", entry.group);
    setValue("color", entry.color);
    setValue("alert", entry.alert);
    setValue("alertValue", entry.alertValue);
    setValue("timer", entry.timer);
    setValue("timerValue", entry.timerValue);
    setValue("timerAction", entry.timerAction);
    setValue("options", entry.options);
    endArray();

    setValue(group + "/size", size);
}

QList<HighlightSettingsEntry> HighlightSettings::getSettings(QString group) {
    int size = beginReadArray(group);

    QList<HighlightSettingsEntry> settings;
    for (int i = 0; i < size; i++) {
        setArrayIndex(i);

        settings.append(HighlightSettingsEntry((const int&)i,
            (const QString&)value("value").toString(),
            (const QString&)value("group").toString(),
            (const QColor&)value("color").value<QColor>(),
            (const bool&)value("alert").toBool(),
            (const QString&)value("alertValue").toString(),
            (const bool&)value("timer").toBool(),
            (const int&)value("timerValue").toInt(),
            (const QString&)value("timerAction").toString(),
            (const QBitArray&)value("options").value<QBitArray>()));
    }
    endArray();

    return settings;
}

void HighlightSettings::setSettings(QString group, QList<HighlightSettingsEntry> settingsList) {
    beginWriteArray(group);
    clear();

    for (int i = 0; i < settingsList.size(); ++i) {
        HighlightSettingsEntry entry = settingsList.at(i);

        setArrayIndex(i);
        setValue("value", entry.value);
        setValue("group", entry.group);
        setValue("color", entry.color);
        setValue("alert", entry.alert);
        setValue("alertValue", entry.alertValue);
        setValue("timer", entry.timer);
        setValue("timerValue", entry.timerValue);
        setValue("timerAction", entry.timerAction);
        setValue("options", entry.options);
    }
    endArray();
}
