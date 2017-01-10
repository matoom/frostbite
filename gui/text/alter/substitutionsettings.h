#ifndef SUBSTITUTIONSETTINGS_H
#define SUBSTITUTIONSETTINGS_H

#include <text/alter/altersettingsentry.h>
#include <clientsettings.h>

class SubstitutionSettings {

public:
    explicit SubstitutionSettings();
    ~SubstitutionSettings();

    void setParameter(AlterSettingsEntry entry);
    void addParameter(AlterSettingsEntry entry);
    QList<AlterSettingsEntry> getSubstitutions();
    void setSettings(QList<AlterSettingsEntry> entries);

private:
    void init();

    QSettings* settings;

signals:

public slots:

};

#endif // SUBSTITUTIONSETTINGS_H
