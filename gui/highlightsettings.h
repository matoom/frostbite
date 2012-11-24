#ifndef HIGHLIGHTSETTINGS_H
#define HIGHLIGHTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <highlightsettingsentry.h>

class HighlightSettings : QSettings {

public:
    HighlightSettings();

    void setSingleParameter(QString name, QVariant value);
    QVariant getSingleParameter(QString name, QVariant defaultValue);

    void setParameter(QString, HighlightSettingsEntry entry);
    void addParameter(QString, HighlightSettingsEntry entry);
    QList<HighlightSettingsEntry> getSettings(QString);
    void setSettings(QString, QList<HighlightSettingsEntry>);

private:

};

#endif // HIGHLIGHTSETTINGS_H
