#include "mainlogger.h"

MainLogger::MainLogger(QObject*) {
    rxRemoveTags.setPattern("<[^>]*>|\\s+$");
    prevType = '\0';
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
    if(logEntry.type == COMMAND && prevType == PROMPT) {
        logger()->info(TextUtils::htmlToPlain(logEntry.text.remove(QRegExp("[\\n]")).remove(rxRemoveTags)).prepend(">"));
    } else {
        if (logEntry.type != PROMPT) {
            if(prevType == PROMPT) {
                logger()->info(">");
            }
            logger()->info(TextUtils::htmlToPlain(logEntry.text.remove(QRegExp("[\\n]")).remove(rxRemoveTags)));
        }
    }
    prevType = logEntry.type;
}

MainLogger::~MainLogger() {
}
