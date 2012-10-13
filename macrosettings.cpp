#include "macrosettings.h"

MacroSettings* MacroSettings::m_pInstance = NULL;

MacroSettings* MacroSettings::Instance() {
    if (!m_pInstance) {
        m_pInstance = new MacroSettings;
    }

    return m_pInstance;
}

MacroSettings::MacroSettings() : QSettings("macros.ini", QSettings::IniFormat) {
    //m_sSettingsFile = QApplication::applicationDirPath().left(1) + ":/demosettings.ini";
}

void MacroSettings::setParameter(QString name, QVariant value) {
    setValue(name, value);
}

QVariant MacroSettings::getParameter(QString name, QVariant defaultValue) {
    return value(name, defaultValue);
}

bool MacroSettings::hasValue(QString value) {
    return contains(value);
}
