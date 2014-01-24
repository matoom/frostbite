#include "deathslogger.h"

DeathsLogger::DeathsLogger(QObject *parent) {
}

void DeathsLogger::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void DeathsLogger::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        logger()->info(localData);
    }
}

DeathsLogger::~DeathsLogger() {

}
