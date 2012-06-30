#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include <QSettings>
#include <QApplication>

class ClientSettings : QSettings {

public:
    static ClientSettings* Instance();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

private:
    ClientSettings();
    ClientSettings(ClientSettings const& copy);
    ClientSettings& operator = (ClientSettings const& copy);
    static ClientSettings* m_pInstance;
};

#endif // CLIENTSETTINGS_H
