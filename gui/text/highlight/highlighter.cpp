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
            HighlightSettingsEntry entry = highlightList.at(i);
            TextUtils::plainToHtml(entry.value);
            // 0 - entire row; 1 - partial words; 2 - starting with; 3 - match groups; 4 - case insensitive
            if(entry.options.at(1)) {
                // do not match inside tags
                rx.setPattern(entry.value % "(?=[^>]*(<|$))");
            } else {
                // do not match inside tags                 
                rx.setPattern("\\b" % entry.value % "\\b(?=[^>]*(<|$))");
            }
            rx.setCaseSensitivity(entry.options.at(4) ? Qt::CaseInsensitive : Qt::CaseSensitive);

            int index = rx.indexIn(text);
            if(index != -1) {
                int count = rx.captureCount();
                if(count == 0 || !entry.options.at(3)) {
                    int pos = this->highlightText(entry, text, index, rx.cap(0));
                    while ((pos = rx.indexIn(text, pos)) != -1) {
                        pos += this->highlightText(entry, text, pos, rx.cap(0));
                    }
                } else {
                    int inserted = 0;
                    for(int i = 1; i < count + 1; i++) {
                        inserted += this->highlightText(entry, text, rx.pos(i) + inserted, rx.cap(i));
                    }
                }
                this->highlightAlert(entry);
                this->highlightTimer(entry);
            }
        }
    }
    return text;
}

int Highlighter::highlightText(HighlightSettingsEntry entry, QString &text, int indexStart, QString match) {
    QString startTag;
    if(entry.bgColor.isValid()) {
        startTag = "<span style=\"color:" % entry.color.name() % ";background:" % entry.bgColor.name()% ";\">";
    } else {
        startTag = "<span style=\"color:" % entry.color.name() % ";\">";
    }
    QString endTag = "</span>";

    int startTagLength = startTag.length();
    int indexEnd = indexStart + startTagLength + match.length();
    //entire row
    if(entry.options.at(0) && !entry.options.at(3)) {
        indexEnd = text.length() + startTagLength;
        // starting with
        if(!entry.options.at(2)) {
            indexStart = 0;
        }
    }
    text.insert(indexStart, startTag);
    text.insert(indexEnd, endTag);

    return startTagLength + endTag.length();
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
