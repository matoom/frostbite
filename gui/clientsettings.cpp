#include "clientsettings.h"

ClientSettings* ClientSettings::m_pInstance = NULL;

ClientSettings* ClientSettings::Instance() {
    if (!m_pInstance) {
        m_pInstance = new ClientSettings;
    }

    return m_pInstance;
}

ClientSettings::ClientSettings() : QSettings("client.ini", QSettings::IniFormat) {
}

void ClientSettings::setParameter(QString name, QVariant value) {
    setValue(name, value);
}

QVariant ClientSettings::getParameter(QString name, QVariant defaultValue) {
    return value(name, defaultValue);
}

bool ClientSettings::hasValue(QString value) {
    return contains(value);
}
