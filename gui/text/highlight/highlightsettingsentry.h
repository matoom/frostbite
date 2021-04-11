#ifndef HIGHLIGHTSETTINGSENTRY_H
#define HIGHLIGHTSETTINGSENTRY_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QColor>
#include <QBitArray>

class HighlightSettingsEntry {

public:
    HighlightSettingsEntry();

    HighlightSettingsEntry(const int &id,
                           const QString &value,
                           const QString &group,
                           const QColor &color,
                           const QColor &bgColor,
                           const bool &alert,
                           const QString &alertValue,
                           const bool &timer,
                           const int &timerValue,
                           const QString &timerAction,
                           const QBitArray &options);

    int id;
    QString value;
    QString group;
    QColor color;
    QColor bgColor;
    bool alert;
    QString alertValue;
    bool timer;
    int timerValue;
    QString timerAction;
    QBitArray options;

    const QString toString();
};

typedef QList<HighlightSettingsEntry> SettingsEntryList;

#endif // HIGHLIGHTSETTINGSENTRY_H
