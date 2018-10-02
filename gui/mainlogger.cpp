#include "mainlogger.h"

MainLogger::MainLogger(QObject*) {
    alter = new Alter();

    rxRemoveTags.setPattern("<[^>]*>|\\s+$");
    prevType = '\0';
}

void MainLogger::updateSettings() {
    alter->reloadSettings();
}

void MainLogger::addText(QString text, char type) {
    LogEntry logEntry = {text, type};

    mMutex.lock();
    dataQueue.enqueue(logEntry);
    mMutex.unlock();
}

void MainLogger::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        log(localData);
    }
}

void MainLogger::log(LogEntry logEntry) {
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

MainLogger::~MainLogger() {
}
