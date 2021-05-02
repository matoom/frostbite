#include "scriptwriterthread.h"
#include "scriptservice.h"
#include "textutils.h"

ScriptWriterThread::ScriptWriterThread(QObject *parent) {
    scriptService = (ScriptService*)parent;

    rxRemoveTags.setPattern("<[^>]*>");

    connect(this, SIGNAL(writeText(QByteArray)),
            scriptService, SLOT(writeOutgoingMessage(QByteArray)));
}

void ScriptWriterThread::onProcess(const QString& lines) {
    foreach (QString line, lines.split("\n")) {
        line = line.remove(rxRemoveTags);
        TextUtils::htmlToPlain(line);
        emit writeText(line.toLocal8Bit());
    }
}

