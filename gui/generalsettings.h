#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QDir>

#include <clientsettings.h>
#include <defaultvalues.h>

class GeneralSettings {

public:
    GeneralSettings();
    ~GeneralSettings();

    void init();
    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    QColor gameWindowFontColor();
    QFont gameWindowFont();
    QColor gameWindowBackground();

    QColor dockWindowFontColor();
    QFont dockWindowFont();
    QColor dockWindowBackground();

private:
    QSettings* settings;

    ClientSettings* clientSettings;

};

#endif // GENERALSETTINGS_H
