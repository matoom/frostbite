#include "highlighter.h"

Highlighter::Highlighter(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    highlightSettings = HighlightSettings::Instance();
    audioPlayer = AudioPlayer::Instance();

    healthAlert = true;
}

QString Highlighter::highlight(QString text) {
    if(!text.isEmpty()) {
        QList<HighlightSettingsEntry> highlightList = highlightSettings->getSettings("TextHighlight");

        HighlightSettingsEntry entry;
        foreach(entry, highlightList){
            // match whole or partial words
            QRegExp rx(QRegExp("\\b" + entry.value + "\\b"));
            if(entry.options.at(1)) {
                rx.setPattern(entry.value);
            }

            int indexStart = text.indexOf(rx);
            if(indexStart != -1) {
                QString startTag = "<span style=\"color:" + entry.color.name() + ";\">";
                QString endTag = "</span>";

                int indexEnd = indexStart + startTag.length() + entry.value.length();
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
                    text.replace(rx, startTag + entry.value + endTag);
                }

                // play alert
                this->highlightAlert(entry);

                // start timer
                this->highlightTimer(entry);
            }
        }
    }
    return text;
}

void Highlighter::highlightAlert(HighlightSettingsEntry entry) {
    if(entry.alert) {
        audioPlayer->play(entry.alertValue);
    }
}

void Highlighter::highlightTimer(HighlightSettingsEntry entry) {
    if(entry.timer) {
        if (entry.timerAction == "Ignore") {
            if(!mainWindow->getTimerBar()->isActive()){
                mainWindow->getTimerBar()->setTimer(entry.timerValue);
            }
        } else {
            mainWindow->getTimerBar()->setTimer(entry.timerValue);
        }
    }
}

void Highlighter::alert(QString eventName, int value) {
    if(eventName == "health") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/health/enabled", false).toBool();
        if(enabled) {
            int settingsValue = highlightSettings->getSingleParameter("AlertHighlight/health/value", 99).toInt();
            if(settingsValue + 1 <= value) {
                if(healthAlert) {
                    QString fileName = highlightSettings->getSingleParameter("AlertHighlight/health/file", "").toString();
                    audioPlayer->play(fileName);
                    healthAlert = false;
                }
            } else {
                healthAlert = true;
            }
        }
    } else if (eventName == "IconDEAD") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/death/enabled", false).toBool();
        if(enabled) {
            audioPlayer->play(highlightSettings->getSingleParameter("AlertHighlight/death/file", "").toString());
        }
    } else if (eventName == "IconSTUNNED") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/stun/enabled", false).toBool();
        if(enabled) {
            audioPlayer->play(highlightSettings->getSingleParameter("AlertHighlight/stun/file", "").toString());
        }
    } else if (eventName == "IconBLEEDING") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/bleeding/enabled", false).toBool();
        if(enabled) {
            audioPlayer->play(highlightSettings->getSingleParameter("AlertHighlight/bleeding/file", "").toString());
        }
    } else if (eventName == "IconWEBBED") {
        bool enabled = highlightSettings->getSingleParameter("AlertHighlight/webbed/enabled", false).toBool();
        if(enabled) {
            audioPlayer->play(highlightSettings->getSingleParameter("AlertHighlight/webbed/file", "").toString());
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
