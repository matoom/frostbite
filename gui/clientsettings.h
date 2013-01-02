#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDockWidget>
#include <QDebug>

#include <QDir>

class ClientSettings : QSettings {

public:
    static ClientSettings* Instance();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);
    QString profilePath();

private:
    ClientSettings();
    ClientSettings(ClientSettings const& copy);
    ClientSettings& operator = (ClientSettings const& copy);
    static ClientSettings* m_pInstance;        
};

#endif // CLIENTSETTINGS_H
