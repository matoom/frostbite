#include "mainlogger.h"

MainLogger::MainLogger(QObject *parent) {
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
    if(logEntry.type == 'c' && prevType == 'p') {
        logEntry.text.remove(rxRemoveTags).replace("&lt;", "<").replace("&amp;", "&");
    } else {
        logEntry.text.remove(rxRemoveTags).replace("&lt;", "<").replace("&amp;", "&").prepend("\n");
    }

    logger()->info(logEntry.text);

    prevType = logEntry.type;
}

MainLogger::~MainLogger() {
}
