#include "apisettings.h"

ApiSettings::ApiSettings() : QSettings(QApplication::applicationDirPath() + "/api.ini", QSettings::IniFormat) {
}

void ApiSettings::setParameter(QString name, QVariant value) {
    setValue(name, value);
}

QVariant ApiSettings::getParameter(QString name, QVariant defaultValue) {
    return value(name, defaultValue);
}
