#ifndef HIGHLIGHTSETTINGS_H
#define HIGHLIGHTSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <highlightsettingsentry.h>

class HighlightSettings : QSettings {

public:
    static HighlightSettings* Instance();

    void setParameter(QString name, HighlightSettingsEntry entry);
    QVariant getParameter(QString name, QVariant defaultValue);

    void addParameter(HighlightSettingsEntry entry);

private:
    HighlightSettings();
    HighlightSettings(HighlightSettings const& copy);
    HighlightSettings& operator = (HighlightSettings const& copy);
    static HighlightSettings* m_pInstance;

    int getLowestFreeId(QStringList list);
};

#endif // HIGHLIGHTSETTINGS_H
