#include "windowwriterthread.h"

WindowWriterThread::WindowWriterThread(QObject *parent, WindowInterface* window) {
    mainWindow = (MainWindow*)parent;
    this->textEdit = dynamic_cast<QPlainTextEdit*>(window);
    this->window = window;

    highlighter = new Highlighter(parent);

    this->exit = false;

    GenericWindow* genericWindow = dynamic_cast<GenericWindow*>(window);
    if(genericWindow != NULL) {
        connect(this, SIGNAL(writeStream(const QString&)), genericWindow, SLOT(appendHtmlStream(const QString&)));
    }

    connect(this, SIGNAL(writeText(const QString&)), this->textEdit, SLOT(appendHtml(const QString&)));
    connect(this, SIGNAL(clearText()), this->textEdit, SLOT(clear()));
}

void WindowWriterThread::updateSettings() {
    highlighter->reloadSettings();
}

void WindowWriterThread::addText(QString text) {
    mMutex.lock();
    dataQueue.enqueue(text);
    mMutex.unlock();
}

void WindowWriterThread::run() {
    while(!this->exit) {
        while(!dataQueue.isEmpty()) {
            mMutex.lock();
            localData = dataQueue.dequeue();
            mMutex.unlock();
            process(localData);
        }
        msleep(200);
    }
}

void WindowWriterThread::process(QString data) {
    if(window->stream()) {
        if(data.startsWith("{clear}")) {
            emit clearText();
        } else {
            emit writeStream(highlighter->highlight(data));
        }
    } else if(window->append()) {
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

void WindowWriterThread::setText(QString text) {
    emit writeText("<span class=\"body\">"
                   + (text.isEmpty() ? "&nbsp;" : text) +
                   "</span>");
}

WindowWriterThread::~WindowWriterThread() {
    this->exit = true;
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
    delete highlighter;
}
