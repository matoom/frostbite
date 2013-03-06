#include "scriptwriterthread.h"

ScriptWriterThread::ScriptWriterThread(QObject *parent) {
    scriptService = (ScriptService*)parent;

    rxRemoveTags.setPattern("<[^>]*>");

    connect(this, SIGNAL(writeText(QByteArray)),
            scriptService, SLOT(writeOutgoingMessage(QByteArray)));
}

void ScriptWriterThread::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void ScriptWriterThread::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        process(localData);
    }
}

void ScriptWriterThread::process(QString line) {
    emit writeText(line.remove(rxRemoveTags).toLocal8Bit());
}

ScriptWriterThread::~ScriptWriterThread() {
}
