#include "clientsettings.h"

ClientSettings* ClientSettings::m_pInstance = NULL;

ClientSettings* ClientSettings::Instance() {
    if (!m_pInstance) {
        m_pInstance = new ClientSettings;
    }

    return m_pInstance;
}

ClientSettings::ClientSettings() : QSettings(QApplication::applicationDirPath() + "/client.ini", QSettings::IniFormat) {
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

QString ClientSettings::profilePath() {
    QString profile = value("Profile/name", "").toString();

    if(!profile.isEmpty()) {
        QDir dir(QApplication::applicationDirPath()  + "/profiles/" + profile);

        if(dir.exists()) {
            return dir.absolutePath() + "/";
        }
    }
    return "";
}
