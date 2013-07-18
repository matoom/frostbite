#include "highlighterthread.h"

HighlighterThread::HighlighterThread(QObject *parent, QPlainTextEdit* textEdit, bool append) {
    mainWindow = (MainWindow*)parent;
    this->textEdit = textEdit;
    this->append = append;

    highlighter = new Highlighter(parent);

    connect(this, SIGNAL(writeText(const QString&)), textEdit, SLOT(appendHtml(const QString&)));
    connect(this, SIGNAL(clearText()), textEdit, SLOT(clear()));
    connect(this, SIGNAL(setScrollBarValue(int)), textEdit->verticalScrollBar(), SLOT(setValue(int)));
}

void HighlighterThread::updateSettings() {
    highlighter->reloadSettings();
}

void HighlighterThread::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void HighlighterThread::run() {
    if(!append) {
        scrollValue = textEdit->verticalScrollBar()->value();
        scrollMax = textEdit->verticalScrollBar()->maximum();
    }

    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        process(localData);
    }
}

void HighlighterThread::process(QString data) {
    if(append) {
        setText(highlighter->highlight(data));
    } else {
        QString text = "";
        QList<QString> lines = data.split('\n');

        int size = lines.size() - 1;
        for(int i = 0; i < size; ++i) {
            text += highlighter->highlight(lines.at(i));

            if(i < size - 1) {
                text += "\n";
            }
        }

        emit clearText();
        setText(text);
        if(scrollValue != scrollMax) {
            emit setScrollBarValue(scrollValue);
        }
    }
}

void HighlighterThread::setText(QString text) {
    if(text.isEmpty()) {
        text = "&nbsp;";
    }

    emit writeText("<span class=\"body\">"
                   + text +
                   "</span>");
}

HighlighterThread::~HighlighterThread() {
    delete highlighter;
}
