#include "scriptwriterthread.h"
#include "scriptservice.h"
#include "textutils.h"

ScriptWriterThread::ScriptWriterThread(QObject *parent) {
    scriptService = (ScriptService*)parent;

    rxRemoveTags.setPattern("<[^>]*>");

    connect(this, SIGNAL(writeText(QByteArray)),
            scriptService, SLOT(writeOutgoingMessage(QByteArray)));
}

void ScriptWriterThread::addText(QString text) {
    dataQueue.push(text);
}

void ScriptWriterThread::run() {
    while(true) {
        QString localData;
        if (dataQueue.waitAndPop(localData)) {
            process(localData);
        } else {
            break;
        }
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
    dataQueue.stop();
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
}
