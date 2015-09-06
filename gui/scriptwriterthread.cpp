#include "scriptwriterthread.h"

ScriptWriterThread::ScriptWriterThread(QObject *parent) {
    scriptService = (ScriptService*)parent;
    exit = false;

    rxRemoveTags.setPattern("<[^>]*>");

    connect(this, SIGNAL(writeText(QByteArray)),
            scriptService, SLOT(writeOutgoingMessage(QByteArray)));
}

void ScriptWriterThread::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
    Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("AddText scriptwriter: " + text);
}

void ScriptWriterThread::run() {
    while(!this->exit) {
        while(!dataQueue.isEmpty()) {
            mMutex.lock();
            localData = dataQueue.dequeue();
            mMutex.unlock();
            process(localData);
        }
        usleep(100);
    }
}

void ScriptWriterThread::process(QString line) {
    Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("process start .. ");
    emit writeText(line.remove(rxRemoveTags).toLocal8Bit());    
    Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("process end");
}

ScriptWriterThread::~ScriptWriterThread() {
    this->exit = true;
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
}
