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

void ScriptWriterThread::process(QString lines) {
    foreach (QString line, lines.split("\n")) {
        line = line.remove(rxRemoveTags);
        TextUtils::htmlToPlain(line);
        emit writeText(line.toLocal8Bit());
    }
}

ScriptWriterThread::~ScriptWriterThread() {
    this->exit = true;
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
}
