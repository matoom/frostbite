#include "mainlogger.h"

#include "defaultvalues.h"
#include "textutils.h"
#include "text/alter/alter.h"

MainLogger::MainLogger(QObject*) {
    alter = new Alter();
    connect(this, &MainLogger::finished, alter, &QObject::deleteLater);

    rxRemoveTags.setPattern("<[^>]*>|\\s+$");
    prevType = '\0';
}

void MainLogger::updateSettings() {
    alter->reloadSettings();
}

void MainLogger::addText(QString text, char type) {
    Parent::addData({text, type});
}

void MainLogger::onProcess(const LogEntry& entry) {
    LogEntry logEntry{entry};
    QString text = TextUtils::htmlToPlain(logEntry.text.remove(rxRemoveTags));
    if(alter->ignore(text, WINDOW_TITLE_MAIN)) return;
    text = alter->substitute(text, WINDOW_TITLE_MAIN);
    if(logEntry.type == COMMAND && prevType == PROMPT) {
        logger()->info(text.prepend(">"));
    } else {
        if (logEntry.type != PROMPT) {
            if(prevType == PROMPT) {
                logger()->info(">");
            }
            logger()->info(text);
        }
    }
    prevType = logEntry.type;
}
