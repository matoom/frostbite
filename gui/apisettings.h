#ifndef APISETTINGS_H
#define APISETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDockWidget>
#include <QDebug>

#include <QDir>

class ApiSettings : QSettings {

public:
    ApiSettings();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

private:

};

#endif // APISETTINGS_H
