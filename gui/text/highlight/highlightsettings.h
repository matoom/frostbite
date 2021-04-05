#ifndef HIGHLIGHTSETTINGS_H
#define HIGHLIGHTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QReadWriteLock>
#include <QMutex>

#include "text/highlight/highlightsettingsentry.h"

class ClientSettings;

class HighlightSettings {
    friend class HighlightSettingsInstance;

public:
    static HighlightSettings* getInstance();
    ~HighlightSettings();

    void setSingleParameter(QString name, QVariant value);
    QVariant getSingleParameter(QString name, QVariant defaultValue);

    void reInit();

    void setParameter(QString, HighlightSettingsEntry entry);
    void addParameter(QString, HighlightSettingsEntry entry);
    QList<HighlightSettingsEntry> getTextHighlights();
    void setSettings(QString, QList<HighlightSettingsEntry>);

private:
    explicit HighlightSettings();

    void create();

    ClientSettings* clientSettings;
    QSettings* settings;

    //QReadWriteLock lock;
    QMutex m_mutex;

public slots:
    QList<HighlightSettingsEntry> loadSettings(QString);
};

class HighlightSettingsInstance : public HighlightSettings {
};


#endif // HIGHLIGHTSETTINGS_H
