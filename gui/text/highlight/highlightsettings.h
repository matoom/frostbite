#ifndef HIGHLIGHTSETTINGS_H
#define HIGHLIGHTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QReadWriteLock>

#include <text/highlight/highlightsettingsentry.h>
#include <clientsettings.h>

class HighlightSettings {

public:
    HighlightSettings();
    ~HighlightSettings();

    void setSingleParameter(QString name, QVariant value);
    QVariant getSingleParameter(QString name, QVariant defaultValue);

    QList<HighlightSettingsEntry>* settingsCache;

    void init();
    void setParameter(QString, HighlightSettingsEntry entry);
    void addParameter(QString, HighlightSettingsEntry entry);
    QList<HighlightSettingsEntry>* getTextHighlights();
    void setSettings(QString, QList<HighlightSettingsEntry>*);

    bool initSettings;

private:
    void create();

public slots:
    void loadSettings(QString, QList<HighlightSettingsEntry>*);

private:
    QSettings* settings;

};

#endif // HIGHLIGHTSETTINGS_H
