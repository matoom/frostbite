#include "macrosettings.h"

MacroSettings* MacroSettings::m_pInstance = NULL;

MacroSettings* MacroSettings::Instance() {
    if (!m_pInstance) {
        m_pInstance = new MacroSettings;
    }
    return m_pInstance;
}

MacroSettings::MacroSettings() {
    this->init();
}

void MacroSettings::init() {
    QString path = ClientSettings::Instance()->profilePath();
    settings = new QSettings(path + "macros.ini", QSettings::IniFormat);
}

void MacroSettings::setParameter(QString name, QVariant value) {
    settings->setValue(name, value);
}

QVariant MacroSettings::getParameter(QString name, QVariant defaultValue) {
    return settings->value(name, defaultValue);
}

bool MacroSettings::hasValue(QString value) {
    return settings->contains(value);
}
