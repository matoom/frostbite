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

    bool isDebug() const {
        return DEBUG;
    }

private:
    explicit ClientSettings();
    // TODO: move to ini or command line?
    bool DEBUG = false;
};

class ClientSettingsInstance : public ClientSettings {
};

#endif // CLIENTSETTINGS_H
