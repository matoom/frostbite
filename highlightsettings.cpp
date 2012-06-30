#include "highlightsettings.h"

HighlightSettings* HighlightSettings::m_pInstance = NULL;

HighlightSettings* HighlightSettings::Instance() {
    if (!m_pInstance) {
        m_pInstance = new HighlightSettings;
    }

    return m_pInstance;
}

HighlightSettings::HighlightSettings() : QSettings("highlights.ini", QSettings::IniFormat) {
    //m_sSettingsFile = QApplication::applicationDirPath().left(1) + ":/demosettings.ini";
}

void HighlightSettings::addParameter(HighlightSettingsEntry entry) {
    qDebug() << entry.id;
    qDebug() << entry.value;
    qDebug() << entry.options;

    /*qDebug() << allKeys();
    beginGroup("TextHighlight");
    qDebug() << childKeys();
    qDebug() << childGroups();
    endGroup();*/

    qDebug() << value("TextHighlight/size");

    /*beginWriteArray("TextHighlight");
    setArrayIndex(0);
    setValue("id", entry.id);
    setValue("value", entry.value);
    setValue("group", entry.group);
    setValue("color", entry.color);
    setValue("timer", entry.timer);
    setValue("timerAction", entry.timerAction);
    setValue("options", entry.options);
    endArray();*/
}

void HighlightSettings::setParameter(QString name, HighlightSettingsEntry entry) {
    QHash<QString, QVariant> settings;
    //settings.insert("key", entry.key);

/*    settings.insert("value", entry.value);
    settings.insert("color", entry.color);
    settings.insert("alert", entry.alert);
    settings.insert("timer", entry.timer);
    settings.insert("options", entry.options);*/

    setValue(name, settings);
}

QVariant HighlightSettings::getParameter(QString name, QVariant defaultValue) {
    return value(name, defaultValue);
}

int HighlightSettings::getLowestFreeId(QStringList list) {
    for (int idx = 0; idx < list.size(); idx++) {
        if (list.at(idx).toInt() == idx) {
            continue;
        } else {
            return idx;
        }
    }

    return list.size();
}
