#include "highlighter.h"

#include <algorithm>
#include <iterator>

#include "text/highlight/highlightsettings.h"
#include "audio/audioplayer.h"
#include "mainwindow.h"
#include "textutils.h"
#include "audio/audioplayer.h"
#include "timerbar.h"
#include "globaldefines.h"



Highlighter::Highlighter(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    audioPlayer = new AudioPlayer(mainWindow);
    reloadSettings();

    healthAlert = true;

    connect(this, SIGNAL(playAudio(QString)), audioPlayer, SLOT(play(QString)));
    connect(this, SIGNAL(setTimer(int)), mainWindow->getTimerBar(), SLOT(setTimer(int)));    
}

void Highlighter::reloadSettings() {
    highlightSettings = HighlightSettings::getInstance();
    auto highlights = highlightSettings->getTextHighlights();
    highlightList.clear();
    std::transform(highlights.begin(), highlights.end(), std::back_inserter(highlightList), &Highlighter::createEntryFromHighlight);
}

Highlighter::Entry Highlighter::createEntryFromHighlight(const HighlightSettingsEntry &highlight) {
    auto htmlValue = highlight.value;
    TextUtils::plainToHtml(htmlValue);
    QRegExp re;
    // 0 - entire row; 1 - partial words; 2 - starting with; 3 - match groups; 4
    // - case insensitive
    if (highlight.options.at(1)) {
        // do not match inside tags
        re.setPattern(htmlValue % "(?=[^>]*(<|$))");
    } else {
        // do not match inside tags
        re.setPattern("\\b" % htmlValue % "\\b(?=[^>]*(<|$))");
    }
    re.setCaseSensitivity(highlight.options.at(4) ? Qt::CaseInsensitive : Qt::CaseSensitive);

    QString endTag = "</span>";
    QString startTag;
    if (highlight.bgColor.isValid()) {
        startTag = "<span style=\"color:" % highlight.color.name() % ";background:"
                % highlight.bgColor.name() % ";\">";
    } else {
        startTag = "<span style=\"color:" % highlight.color.name() % ";\">";
    }

    Entry entry {highlight, htmlValue, startTag, endTag, re};
    return entry;
}

QString Highlighter::highlight(QString text) {        
    if(!text.isEmpty()) {
        for(size_t i = 0; i < highlightList.size(); ++i) {
            // copy regexp since it is mutable
            auto rx = highlightList[i].re;
            
            int index = rx.indexIn(text);
            if(index != -1) {
                int count = rx.captureCount();
                if(count == 0 || !highlightList[i].entry.options.at(3)) {
                    int pos = index;
                    while ((pos = rx.indexIn(text, pos)) != -1) {
                        pos += this->highlightText(highlightList[i], text, pos, rx.cap(0).length());
                    }
                } else {
                    int inserted = 0;
                    for(int i = 1; i < count + 1; i++) {
                        inserted += this->highlightText(highlightList[i], text, rx.pos(i) + inserted, rx.cap(i).length());
                    }
                }
                this->highlightAlert(highlightList[i].entry);
                this->highlightTimer(highlightList[i].entry);
            }
        }
    }
    return text;
}

int Highlighter::highlightText(const Entry& entry, QString &text, int indexStart, int matchLength) {

    int startTagLength = entry.startTag.length();
    int indexEnd = indexStart + startTagLength + matchLength;
    //entire row
    if(entry.entry.options.at(0) && !entry.entry.options.at(3)) {
        indexEnd = text.length() + startTagLength;
        // starting with
        if(!entry.entry.options.at(2)) {
            indexStart = 0;
        }
    }
    text.insert(indexStart, entry.startTag);
    text.insert(indexEnd, entry.endTag);

    return startTagLength + entry.endTag.length();
}

void Highlighter::highlightAlert(const HighlightSettingsEntry& entry) {
    if(entry.alert) {
        emit playAudio(entry.alertValue);
    }
}

void Highlighter::highlightTimer(const HighlightSettingsEntry& entry) {
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
