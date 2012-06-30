#ifndef HIGHLIGHTSETTINGSENTRY_H
#define HIGHLIGHTSETTINGSENTRY_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QColor>

class HighlightSettingsEntry {

public:
    HighlightSettingsEntry(const QString &id, const QString &value, const QString &group, const QColor &color,
                           const QString &alert, const int &timer, const QString &timerAction, const int &options);
    QString id;
    QString value;
    QString group;
    QColor color;
    QString alert;
    int timer;
    QString timerAction;
    int options;
};

typedef QList<HighlightSettingsEntry> SettingsEntryList;

#endif // HIGHLIGHTSETTINGSENTRY_H
