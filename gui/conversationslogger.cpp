#include "conversationslogger.h"

ConversationsLogger::ConversationsLogger(QObject*) {
    rxRemoveTags.setPattern("<[^>]*>");
}

void ConversationsLogger::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void ConversationsLogger::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        log(localData);
    }
}

void ConversationsLogger::log(QString logText) {
    logger()->info(logText.remove(rxRemoveTags));
}

ConversationsLogger::~ConversationsLogger() {
}
