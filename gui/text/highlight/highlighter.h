#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>

#include "text/highlight/highlightsettingsentry.h"

class HighlightSettings;
class MainWindow;
class HighlightSettingsEntry;
class AudioPlayer;

class Highlighter : public QObject {
    Q_OBJECT

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

    int highlightText(HighlightSettingsEntry, QString&, int, QString);
    void highlightAlert(HighlightSettingsEntry);
    void highlightTimer(HighlightSettingsEntry);

    static QString createCommand(const QString& text, const QString& command);

    Qt::CaseSensitivity matchCase(bool);

    QList<HighlightSettingsEntry> highlightList;

signals:
    void playAudio(QString);
    void setTimer(int);
    void updateSettings();
    
public slots:

};

#endif // HIGHLIGHTER_H
