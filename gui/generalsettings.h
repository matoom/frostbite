#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QDir>

#include <clientsettings.h>
#include <defaultvalues.h>

class GeneralSettings {    
    friend class GeneralSettingsInstance;

public:
    static GeneralSettings* getInstance();

    ~GeneralSettings();

    void reInit();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    QColor gameWindowFontColor();
    QFont gameWindowFont();
    QColor gameWindowBackground();

    QColor dockWindowFontColor();
    QFont dockWindowFont();
    QColor dockWindowBackground();

private:
    explicit GeneralSettings();

    void init();

    QSettings* settings;

    ClientSettings* clientSettings;

};

class GeneralSettingsInstance : public GeneralSettings {
};

#endif // GENERALSETTINGS_H
