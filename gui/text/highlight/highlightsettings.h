#ifndef HIGHLIGHTSETTINGS_H
#define HIGHLIGHTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QReadWriteLock>

#include <text/highlight/highlightsettingsentry.h>
#include <clientsettings.h>

class HighlightSettings {
    friend class HighlightSettingsInstance;

public:
    static HighlightSettings* getInstance();
    ~HighlightSettings();

    void setSingleParameter(QString name, QVariant value);
    QVariant getSingleParameter(QString name, QVariant defaultValue);

    QList<HighlightSettingsEntry>* settingsCache;

    void reInit();

    void setParameter(QString, HighlightSettingsEntry entry);
    void addParameter(QString, HighlightSettingsEntry entry);
    QList<HighlightSettingsEntry>* getTextHighlights();
    void setSettings(QString, QList<HighlightSettingsEntry>*);

private:
    explicit HighlightSettings();

    void create();

public slots:
    void loadSettings(QString, QList<HighlightSettingsEntry>*);

private:
    ClientSettings* clientSettings;
    QSettings* settings;

    QReadWriteLock lock;

};

class HighlightSettingsInstance : public HighlightSettings {
};


#endif // HIGHLIGHTSETTINGS_H
