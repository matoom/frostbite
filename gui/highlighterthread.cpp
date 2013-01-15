#include "highlighterthread.h"

HighlighterThread::HighlighterThread(QObject *parent, QPlainTextEdit* textEdit, bool clear) {
    mainWindow = (MainWindow*)parent;
    this->textEdit = textEdit;
    this->clear = clear;

    highlighter = new Highlighter(parent);

    connect(this, SIGNAL(writeText(const QString&)), textEdit, SLOT(appendHtml(const QString&)));
    connect(this, SIGNAL(clearText()), textEdit, SLOT(clear()));
}

void HighlighterThread::updateSettings() {
    highlighter->reloadSettings();
}

void HighlighterThread::addText(QString text) {
    //mMutex.lock();
    dataQueue.enqueue(text);
    //mMutex.unlock();
}

void HighlighterThread::run() {
    while(!dataQueue.isEmpty()) {
        process(dataQueue.dequeue());
    }
}

void HighlighterThread::process(QString data) {
    QList<QString> lines = data.split('\n');

    QString text = "";
    for(int i = 0; i < lines.size(); ++i) {
        if(clear) {
            text += highlighter->highlight(lines.at(i)) + "\n";
        } else {
            text += highlighter->highlight(lines.at(i));
        }
    }

    if(clear){
        emit clearText();
    }
    emit writeText("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + text + "</SPAN>");
}

HighlighterThread::~HighlighterThread() {
    delete highlighter;
}
