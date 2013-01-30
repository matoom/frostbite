#include "highlighterthread.h"

HighlighterThread::HighlighterThread(QObject *parent, QPlainTextEdit* textEdit, bool multiLine) {
    mainWindow = (MainWindow*)parent;
    this->textEdit = textEdit;
    this->multiLine = multiLine;

    highlighter = new Highlighter(parent);

    connect(this, SIGNAL(writeText(const QString&)), textEdit, SLOT(appendHtml(const QString&)));
    connect(this, SIGNAL(clearText()), textEdit, SLOT(clear()));
}

void HighlighterThread::updateSettings() {
    highlighter->reloadSettings();
}

void HighlighterThread::addText(QString text) {
    QWriteLocker locker(&lock);
    dataQueue.enqueue(text);
}

void HighlighterThread::run() {
    QReadLocker locker(&lock);
    while(!dataQueue.isEmpty()) {
        process(dataQueue.dequeue());
    }
}

void HighlighterThread::process(QString data) {
    QString text = "";

    if(multiLine) {
        QList<QString> lines = data.split('\n');

        int size = lines.size() - 1;
        for(int i = 0; i < size; ++i) {
            text += highlighter->highlight(lines.at(i));

            if(i < size) {
                text += "\n";
            }
        }
        emit clearText();
    } else {
        text = highlighter->highlight(data);
    }
    emit writeText("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + text + "</SPAN>");
}

HighlighterThread::~HighlighterThread() {
    delete highlighter;
}
