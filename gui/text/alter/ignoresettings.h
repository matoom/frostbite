#ifndef IGNORESETTINGS_H
#define IGNORESETTINGS_H

#include <QObject>

#include <text/alter/altersettingsentry.h>
#include <clientsettings.h>

class IgnoreSettings {

public:
    IgnoreSettings();
    ~IgnoreSettings();

    void setParameter(AlterSettingsEntry entry);
    void addParameter(AlterSettingsEntry entry);
    QList<AlterSettingsEntry> getIgnores();
    void setSettings(QList<AlterSettingsEntry> entries);

private:
    void init();

    QSettings* settings;
    ClientSettings* clientSettings;
};

#endif // IGNORESETTINGS_H
