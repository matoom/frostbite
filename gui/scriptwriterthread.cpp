#include "scriptwriterthread.h"
#include "scriptservice.h"
#include "textutils.h"

ScriptWriterThread::ScriptWriterThread(QObject *parent) {
    scriptService = (ScriptService*)parent;

    rxRemoveTags.setPattern("<[^>]*>");
}

void ScriptWriterThread::onProcess(const QString& lines) {
    foreach (QString line, lines.split("\n")) {
        line = line.remove(rxRemoveTags);
        TextUtils::htmlToPlain(line);
        emit writeText(line.toLocal8Bit());
    }
}

