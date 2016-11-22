#include "gridwriterthread.h"

GridWriterThread::GridWriterThread(QObject *parent) {
    mainWindow = (MainWindow*)parent;
    highlighter = new Highlighter(parent);

    this->exit = false;
}

void GridWriterThread::updateSettings() {
    highlighter->reloadSettings();
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
            process(localData);
        }
        msleep(200);
    }
}

void GridWriterThread::process(GridEntry gridEntry) {
    if(gridEntry.text.isEmpty()) {
        highlightedItems.remove(gridEntry.name);
    } else {
        highlightedItems.insert(gridEntry.name, highlighter->highlight(gridEntry.text));
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
