#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QRegExp>
#include <vector>

#include "text/highlight/highlightsettingsentry.h"

class HighlightSettings;
class MainWindow;
class AudioPlayer;

class Highlighter : public QObject {
    Q_OBJECT
public:
    struct Entry {
        HighlightSettingsEntry entry;
        QString htmlValue;
        QString startTag;
        QString endTag;
        QRegExp re;
    };
public:
    explicit Highlighter(QObject *parent = 0);
    ~Highlighter();

    QString highlight(QString);
    void alert(QString eventName, int value = 99);
    void reloadSettings();

private:
    HighlightSettings* highlightSettings;
    AudioPlayer* audioPlayer;
    MainWindow* mainWindow;  

    QRegExp rx;

    bool healthAlert;    

    int highlightText(const Entry&, QString&, int, int);
    void highlightAlert(const HighlightSettingsEntry&);
    void highlightTimer(const HighlightSettingsEntry&);

    static Entry createEntryFromHighlight(const HighlightSettingsEntry& highlight);
    
    Qt::CaseSensitivity matchCase(bool);

    std::vector<Entry> highlightList;

signals:
    void playAudio(QString);
    void setTimer(int);
    void updateSettings();
    
public slots:

};

#endif // HIGHLIGHTER_H
