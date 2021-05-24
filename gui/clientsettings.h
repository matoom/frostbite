#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDockWidget>
#include <QDebug>
#include <QStandardPaths>

#include <QDir>

class ClientSettings : QSettings {
    friend class ClientSettingsInstance;

public:
    static ClientSettings* getInstance();
    ~ClientSettings();

    void setParameter(QString name, QVariant value);
    QString getQStringNotBlank(QString name, QVariant defaultValue);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);
    QString profilePath();

    void remove(QString name);

private:
    explicit ClientSettings();

};

class ClientSettingsInstance : public ClientSettings {
};

#endif // CLIENTSETTINGS_H
