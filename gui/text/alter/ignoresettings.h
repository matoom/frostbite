#ifndef IGNORESETTINGS_H
#define IGNORESETTINGS_H

#include <QObject>
#include <QMutex>

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

    void setEnabled(bool enabled);
    bool getEnabled();

    void reInit();

private:        
    explicit IgnoreSettings();

    void create();

    QSettings* settings;
    ClientSettings* clientSettings;

    QMutex m_mutex;
};

class IgnoreSettingsInstance : public IgnoreSettings {
};

#endif // IGNORESETTINGS_H
