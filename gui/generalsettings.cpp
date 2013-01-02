#include "generalsettings.h"

GeneralSettings::GeneralSettings() {
    this->init();
}

void GeneralSettings::init() {
    QString path = ClientSettings::Instance()->profilePath();
    settings = new QSettings(path + "general.ini", QSettings::IniFormat);
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

GeneralSettings::~GeneralSettings() {

}
