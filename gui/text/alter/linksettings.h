#ifndef LINKSETTINGS_H
#define LINKSETTINGS_H

#include <QMutex>
#include <QSettings>

#include "text/alter/altersettingsentry.h"

class ClientSettings;

class LinkSettings {
    friend class LinkSettingsInstance;

public:
    static LinkSettings* getInstance();
    ~LinkSettings();

    void reInit();

    void setParameter(AlterSettingsEntry entry);
    void addParameter(AlterSettingsEntry entry);
    QList<AlterSettingsEntry> getLinks();
    void setSettings(QList<AlterSettingsEntry> entries);
    void loadSettings(QString, QList<AlterSettingsEntry>&);

    void setEnabled(bool enabled);
    bool getEnabled();

private:
    explicit LinkSettings();

    void create();

    QSettings* settings;
    ClientSettings* clientSettings;

    //QReadWriteLock lock;
    QMutex m_mutex;

signals:

public slots:

};

class LinkSettingsInstance : public LinkSettings {
};

#endif // LINKSETTINGS_H
