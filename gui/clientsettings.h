#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDockWidget>
#include <QDebug>

#include <QDir>

class ClientSettings : QSettings {

public:
    ClientSettings();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);
    QString profilePath();

private:

};

#endif // CLIENTSETTINGS_H
