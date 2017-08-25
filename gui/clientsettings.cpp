#include "clientsettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(ClientSettingsInstance, uniqueInstance)

ClientSettings* ClientSettings::getInstance() {
    if(uniqueInstance.exists()) {
        return uniqueInstance;
    } else {
        return new ClientSettingsInstance();
    }
}

ClientSettings::ClientSettings() : QSettings(QApplication::applicationDirPath() + "/client.ini", QSettings::IniFormat) {
}

void ClientSettings::setParameter(QString name, QVariant value) {
    setValue(name, value);
}

QString ClientSettings::getQStringNotBlank(QString name, QVariant defaultValue) {
    QString param = value(name, defaultValue).toString().trimmed();
    if(param.isEmpty()) return defaultValue.toString();
    return param;
}

QVariant ClientSettings::getParameter(QString name, QVariant defaultValue) {
    return value(name, defaultValue);
}

bool ClientSettings::hasValue(QString value) {
    return contains(value);
}

QString ClientSettings::profilePath() {
    QString profile = value("Profile/name", "").toString();
    QString type = value("Profile/type", "").toString();

    if(!profile.isEmpty()) {        
        QDir dir;
        if(type == "H") {
            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/profiles/" + profile);
        } else {
            dir = QDir(QApplication::applicationDirPath() + "/profiles/" + profile);
        }

        if(dir.exists()) {
            return dir.absolutePath() + "/";
        }
    }
    return "";
}
