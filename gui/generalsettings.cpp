#include "generalsettings.h"

#include <QGlobalStatic>

#include "clientsettings.h"
#include "defaultvalues.h"


Q_GLOBAL_STATIC(GeneralSettingsInstance, uniqueInstance)

GeneralSettings* GeneralSettings::getInstance() {
    return uniqueInstance;
}

GeneralSettings::GeneralSettings() {
    clientSettings = ClientSettings::getInstance();
    this->init();
}

void GeneralSettings::init() {    
    settings = new QSettings(clientSettings->profilePath() + "general.ini", QSettings::IniFormat);
}

void GeneralSettings::reInit() {
    delete settings;
    this->init();
}

void GeneralSettings::setParameter(QString name, QVariant value) {
    settings->setValue(name, value);
}

QVariant GeneralSettings::getParameter(QString name, QVariant defaultValue) {
    return settings->value(name, defaultValue);
}

QColor GeneralSettings::gameWindowFontColor() {
    return settings->value("GameWindow/fontColor",
        DEFAULT_MAIN_FONT_COLOR).value<QColor>();
}

QFont GeneralSettings::gameWindowFont(){
    return settings->value("GameWindow/font",
        QFont(DEFAULT_MAIN_FONT, DEFAULT_MAIN_FONT_SIZE)).value<QFont>();
}

QColor GeneralSettings::gameWindowBackground(){
    return settings->value("GameWindow/background",
        DEFAULT_MAIN_BACKGROUND).value<QColor>();
}

QColor GeneralSettings::dockWindowFontColor(){
    return settings->value("DockWindow/fontColor",
        DEFAULT_DOCK_FONT_COLOR).value<QColor>();
}

QFont GeneralSettings::dockWindowFont(){
    return settings->value("DockWindow/font",
        QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE)).value<QFont>();
}

QColor GeneralSettings::dockWindowBackground(){
    return settings->value("DockWindow/background",
        DEFAULT_DOCK_BACKGROUND).value<QColor>();
}

QColor GeneralSettings::cmdFontColor(){
    return settings->value("Commandline/fontColor",
        DEFAULT_CMD_FONT_COLOR).value<QColor>();
}

QFont GeneralSettings::cmdFont(){
    return settings->value("Commandline/font",
        QFont(DEFAULT_CMD_FONT, DEFAULT_CMD_FONT_SIZE)).value<QFont>();
}

QColor GeneralSettings::cmdBackground(){
    return settings->value("Commandline/background",
        DEFAULT_CMD_BACKGROUND).value<QColor>();
}

QColor GeneralSettings::cmdRtColor(){
    return settings->value("Commandline/rtColor",
        DEFAULT_CMD_RT_COLOR).value<QColor>();
}

QColor GeneralSettings::cmdCtColor(){
    return settings->value("Commandline/ctColor",
        DEFAULT_CMD_CT_COLOR).value<QColor>();
}

GeneralSettings::~GeneralSettings() {    
    delete settings;
}
