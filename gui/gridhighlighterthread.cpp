#include "gridhighlighterthread.h"

GridHighlighterThread::GridHighlighterThread(QObject *parent) {
    mainWindow = (MainWindow*)parent;
    highlighter = new Highlighter(parent);

    this->exit = false;
}

void GridHighlighterThread::updateSettings() {
    highlighter->reloadSettings();
}

void GridHighlighterThread::addItem(QString name, QString text) {
    GridEntry gridEntry = {name, text};
    mMutex.lock();
    dataQueue.enqueue(gridEntry);
    mMutex.unlock();
}

void GridHighlighterThread::run() {
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

void GridHighlighterThread::process(GridEntry gridEntry) {
    if(gridEntry.text.isEmpty()) {
        highlightedItems.remove(gridEntry.name);
    } else {
        highlightedItems.insert(gridEntry.name, highlighter->highlight(gridEntry.text));
    }
    emit writeGrid(highlightedItems);
}

GridHighlighterThread::~GridHighlighterThread() {
    this->exit = true;
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
    delete highlighter;
}
