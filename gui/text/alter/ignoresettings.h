#ifndef IGNORESETTINGS_H
#define IGNORESETTINGS_H

#include <QObject>
#include <QReadWriteLock>

#include <text/alter/altersettingsentry.h>
#include <clientsettings.h>


class IgnoreSettings {
    friend class IgnoreSettingsInstance;

public:
    static IgnoreSettings* getInstance();
    ~IgnoreSettings();

    void setParameter(AlterSettingsEntry entry);
    void addParameter(AlterSettingsEntry entry);
    QList<AlterSettingsEntry> getIgnores();
    void setSettings(QList<AlterSettingsEntry> entries);
    void loadSettings(QString, QList<AlterSettingsEntry>&);

    void reInit();

private:        
    explicit IgnoreSettings();

    void create();

    bool initSettings;    

    QSettings* settings;
    ClientSettings* clientSettings;

    QReadWriteLock lock;
};

class IgnoreSettingsInstance : public IgnoreSettings {
};

#endif // IGNORESETTINGS_H
