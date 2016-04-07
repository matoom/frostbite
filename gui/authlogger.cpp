#include "authlogger.h"

AuthLogger::AuthLogger(QObject*) {
}

void AuthLogger::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void AuthLogger::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        logger()->info(localData);
    }
}

AuthLogger::~AuthLogger() {
}
