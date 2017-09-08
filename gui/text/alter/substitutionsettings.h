#ifndef SUBSTITUTIONSETTINGS_H
#define SUBSTITUTIONSETTINGS_H

#include <QMutex>

#include <text/alter/altersettingsentry.h>
#include <clientsettings.h>

class SubstitutionSettings {
    friend class SubstitutionSettingsInstance;

public:
    static SubstitutionSettings* getInstance();
    ~SubstitutionSettings();

    void reInit();

    void setParameter(AlterSettingsEntry entry);
    void addParameter(AlterSettingsEntry entry);
    QList<AlterSettingsEntry> getSubstitutions();
    void setSettings(QList<AlterSettingsEntry> entries);
    void loadSettings(QString, QList<AlterSettingsEntry>&);

private:
    explicit SubstitutionSettings();

    void create();

    QSettings* settings;
    ClientSettings* clientSettings;

    //QReadWriteLock lock;
    QMutex m_mutex;

signals:

public slots:

};

class SubstitutionSettingsInstance : public SubstitutionSettings {
};

#endif // SUBSTITUTIONSETTINGS_H
