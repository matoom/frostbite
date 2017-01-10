#include "gridwriterthread.h"

GridWriterThread::GridWriterThread(QObject *parent, GridWindow* window) {
    mainWindow = (MainWindow*)parent;
    highlighter = new Highlighter(parent);
    alter = new Alter();

    this->window = window;
    this->exit = false;
}

void GridWriterThread::updateSettings() {
    highlighter->reloadSettings();
    alter->reloadSettings();
}

void GridWriterThread::addItem(QString name, QString text) {
    GridEntry gridEntry = {name, text};
    mMutex.lock();
    dataQueue.enqueue(gridEntry);
    mMutex.unlock();
}

void GridWriterThread::run() {
    while(!this->exit) {
        while(!dataQueue.isEmpty()) {
            mMutex.lock();
            localData = dataQueue.dequeue();
            mMutex.unlock();
            this->write(localData);
        }
        msleep(200);
    }
}

QString GridWriterThread::process(QString text, QString win) {
    return highlighter->highlight(alter->substitute(text, win));
}

void GridWriterThread::write(GridEntry gridEntry) {
    if(alter->ignore(gridEntry.text, window->objectName())) return;

    if(gridEntry.text.isEmpty()) {
        highlightedItems.remove(gridEntry.name);
    } else {
        highlightedItems.insert(gridEntry.name, this->process(gridEntry.text, window->objectName()));
    }
    emit writeGrid(highlightedItems);
}

GridWriterThread::~GridWriterThread() {
    this->exit = true;
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
    delete highlighter;
}
