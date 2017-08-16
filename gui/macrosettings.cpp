#include "macrosettings.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(MacroSettingsInstance, uniqueInstance)

MacroSettings* MacroSettings::getInstance() {
    if(uniqueInstance.exists()) {
        return uniqueInstance;
    } else {
        return new MacroSettingsInstance();
    }
}

MacroSettings::MacroSettings() {
    clientSettings = ClientSettings::getInstance();
    this->init();
}

void MacroSettings::reInit() {
    delete settings;
    this->init();
}

void MacroSettings::init() {
    settings = new QSettings(clientSettings->profilePath() + "macros.ini", QSettings::IniFormat);
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

MacroSettings::~MacroSettings() {
    delete settings;
    delete clientSettings;
}
