#include "debuglogger.h"

DebugLogger::DebugLogger(QObject*) {
}

void DebugLogger::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void DebugLogger::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        logger()->info(localData);
    }
}

DebugLogger::~DebugLogger() {
}
