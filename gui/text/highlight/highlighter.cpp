#include "highlighter.h"

Highlighter::Highlighter(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    highlightSettings = HighlightSettings::getInstance();
    audioPlayer = new AudioPlayer(mainWindow);
    highlightList = highlightSettings->getTextHighlights();

    healthAlert = true;

    connect(this, SIGNAL(playAudio(QString)), audioPlayer, SLOT(play(QString)));
    connect(this, SIGNAL(setTimer(int)), mainWindow->getTimerBar(), SLOT(setTimer(int)));    
}

void Highlighter::reloadSettings() {
    highlightSettings = HighlightSettings::getInstance();
    highlightList = highlightSettings->getTextHighlights();
}

QString Highlighter::highlight(QString text) {
    if(!text.isEmpty()) {
        for(int i = 0; i < highlightList.size(); ++i) {
            HighlightSettingsEntry highlightEntry = highlightList.at(i);
            TextUtils::plainToHtml(highlightEntry.value);

            // match whole or partial words
            if(highlightEntry.options.at(1)) {
                // do not match inside tags
                rx.setPattern(highlightEntry.value + "(?=[^>]*(<|$))");
            } else {
                // do not match inside tags                 
                rx.setPattern("\\b" + highlightEntry.value + "\\b(?=[^>]*(<|$))");
            }

            int indexStart = rx.indexIn(text);
            if(indexStart != -1) {
                // highlight text
                this->highlightText(highlightEntry, text, indexStart, rx.capturedTexts());
                // play alert
                this->highlightAlert(highlightEntry);
                // start timer
                this->highlightTimer(highlightEntry);
            }
        }
    }
    return text;
}

void Highlighter::highlightText(HighlightSettingsEntry entry, QString &text, int indexStart, QStringList capturedTexts) {
    QString startTag = "<span style=\"color:" + entry.color.name() + ";\">";
    QString endTag = "</span>";

    foreach(QString match, capturedTexts) {
        int indexEnd = indexStart + startTag.length() + match.length();
        //entire row
        if(entry.options.at(0)) {
            indexEnd = text.length() + startTag.length();
            // starting with
            if(!entry.options.at(2)) {
                indexStart = 0;
            }
            text.insert(indexStart, startTag);
            text.insert(indexEnd, endTag);
        } else {
            text.replace(rx, startTag + match + endTag);
        }
    }
}

void Highlighter::highlightAlert(HighlightSettingsEntry entry) {
    if(entry.alert) {
        emit playAudio(entry.alertValue);
    }
}

void Highlighter::highlightTimer(HighlightSettingsEntry entry) {
    if(entry.timer) {
        if (entry.timerAction == "Ignore") {
            if(!mainWindow->getTimerBar()->isActive()){
                emit setTimer(entry.timerValue);
            }
        } else {
            emit setTimer(entry.timerValue);
        }
    }
}

void Highlighter::alert(QString eventName, int value) {
    if(eventName == "health") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/health/enabled", false).toBool();
        if(enabled) {
            int settingsValue = highlightSettings->getSingleParameter("AlertHighlight/health/value", 99).toInt();
            if(value <= settingsValue + 1) {
                if(healthAlert) {
                    QString fileName = highlightSettings->getSingleParameter("AlertHighlight/health/file", "").toString();
                    emit playAudio(fileName);
                    healthAlert = false;
                }
            } else {
                healthAlert = true;
            }
        }
    } else if(eventName == "IconDEAD") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/death/enabled", false).toBool();
        if(enabled) {
            emit playAudio(highlightSettings->getSingleParameter("AlertHighlight/death/file", "").toString());
        }
    } else if(eventName == "IconSTUNNED") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/stun/enabled", false).toBool();
        if(enabled) {
            emit playAudio(highlightSettings->getSingleParameter("AlertHighlight/stun/file", "").toString());
        }
    } else if(eventName == "IconBLEEDING") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/bleeding/enabled", false).toBool();
        if(enabled) {
            emit playAudio(highlightSettings->getSingleParameter("AlertHighlight/bleeding/file", "").toString());
        }
    } else if(eventName == "IconWEBBED") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/webbed/enabled", false).toBool();
        if(enabled) {
            emit playAudio(highlightSettings->getSingleParameter("AlertHighlight/webbed/file", "").toString());
        }
    }
}

Qt::CaseSensitivity Highlighter::matchCase(bool value) {
    Qt::CaseSensitivity matchCase = Qt::CaseSensitive;
    if(value){
        matchCase = Qt::CaseInsensitive;
    }
    return matchCase;
}

Highlighter::~Highlighter() {
    delete audioPlayer;
}
