#include "highlightsettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(HighlightSettingsInstance, uniqueInstance)

HighlightSettings* HighlightSettings::getInstance() {
    return uniqueInstance;
}

HighlightSettings::HighlightSettings() {
    clientSettings = ClientSettings::getInstance();
    this->create();
}

void HighlightSettings::reInit() {
    QMutexLocker locker(&m_mutex);
    delete settings;
    this->create();
}

void HighlightSettings::create() {
    settings = new QSettings(clientSettings->profilePath() + "highlights.ini", QSettings::IniFormat);
}

void HighlightSettings::setSingleParameter(QString name, QVariant value) {
    QMutexLocker locker(&m_mutex);
    settings->setValue(name, value);
}

QVariant HighlightSettings::getSingleParameter(QString name, QVariant defaultValue) {
    QMutexLocker locker(&m_mutex);
    return settings->value(name, defaultValue);
}

void HighlightSettings::addParameter(QString group, HighlightSettingsEntry entry) {
    QMutexLocker locker(&m_mutex);
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
    QMutexLocker locker(&m_mutex);
    int size = settings->value(group + "/size").toInt();

    settings->beginWriteArray(group);
    settings->setArrayIndex(entry.id);
    settings->setValue("value", entry.value);
    settings->setValue("group", entry.group);
    settings->setValue("color", entry.color);
    settings->setValue("bgColor", entry.bgColor);
    settings->setValue("alert", entry.alert);
    settings->setValue("alertValue", entry.alertValue);
    settings->setValue("timer", entry.timer);
    settings->setValue("timerValue", entry.timerValue);
    settings->setValue("timerAction", entry.timerAction);
    settings->setValue("options", entry.options);
    settings->endArray();

    settings->setValue(group + "/size", size);
}

QList<HighlightSettingsEntry> HighlightSettings::getTextHighlights() {
    return this->loadSettings("TextHighlight");
}

QList<HighlightSettingsEntry> HighlightSettings::loadSettings(QString group) {
    QMutexLocker locker(&m_mutex);
    QList<HighlightSettingsEntry> settingsList = QList<HighlightSettingsEntry>();
    int size = settings->beginReadArray(group);
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);

        // convert legacy settings
        QBitArray opts = settings->value("options", QBitArray()).value<QBitArray>();
        if(opts.size() < 5) opts.resize(5);

        settingsList.append(HighlightSettingsEntry((const int&)i,
                (const QString&)settings->value("value", "").toString(),
                (const QString&)settings->value("group", "").toString(),
                (const QColor&)settings->value("color", QColor()).value<QColor>(),
                (const QColor&)settings->value("bgColor", QColor()).value<QColor>(),
                (const bool&)settings->value("alert", false).toBool(),
                (const QString&)settings->value("alertValue", "").toString(),
                (const bool&)settings->value("timer", false).toBool(),
                (const int&)settings->value("timerValue", 0).toInt(),
                (const QString&)settings->value("timerAction", "").toString(),
                (const QBitArray&)opts));
    }
    settings->endArray();
    return settingsList;
}

void HighlightSettings::setSettings(QString group, QList<HighlightSettingsEntry> settingsList) {
    QMutexLocker locker(&m_mutex);
    settings->remove(group);
    settings->beginWriteArray(group);

    for (int i = 0; i < settingsList.size(); ++i) {
        HighlightSettingsEntry entry = settingsList.at(i);

        settings->setArrayIndex(i);
        settings->setValue("value", entry.value);
        settings->setValue("group", entry.group);
        settings->setValue("color", entry.color);
        settings->setValue("bgColor", entry.bgColor);
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
}
