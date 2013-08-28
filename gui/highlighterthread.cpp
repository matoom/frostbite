#include "highlighterthread.h"

HighlighterThread::HighlighterThread(QObject *parent, WindowInterface* window) {
    mainWindow = (MainWindow*)parent;
    this->textEdit = dynamic_cast<QPlainTextEdit*>(window);
    this->window = window;
    this->append = append;

    highlighter = new Highlighter(parent);

    connect(this, SIGNAL(writeText(const QString&)), this->textEdit, SLOT(appendHtml(const QString&)));
    connect(this, SIGNAL(clearText()), this->textEdit, SLOT(clear()));
    connect(this, SIGNAL(setScrollBarValue(int)), this->textEdit->verticalScrollBar(), SLOT(setValue(int)));
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
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        process(localData);
    }
}

void HighlighterThread::process(QString data) {
    if(window->append()) {
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
    }
}

void HighlighterThread::setText(QString text) {
    emit writeText("<span class=\"body\">"
                   + (text.isEmpty() ? "&nbsp;" : text) +
                   "</span>");
}

HighlighterThread::~HighlighterThread() {
    delete highlighter;
}
