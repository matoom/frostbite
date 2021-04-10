#include "gridwriterthread.h"
#include "mainwindow.h"
#include "gridwindow.h"
#include "text/alter/alter.h"
#include "text/highlight/highlighter.h"

GridWriterThread::GridWriterThread(QObject *parent, GridWindow* window) {
    mainWindow = (MainWindow*)parent;
    highlighter = new Highlighter(parent);
    alter = new Alter();

    this->window = window;
}

void GridWriterThread::updateSettings() {
    highlighter->reloadSettings();
    alter->reloadSettings();
}

void GridWriterThread::addItem(QString name, QString text) {
    GridEntry gridEntry = {name, text};
    dataQueue.push(gridEntry);
}

void GridWriterThread::run() {
    while (!this->isInterruptionRequested()) {
        GridEntry localData;
        if (dataQueue.waitAndPop(localData)) {
            write(localData);
        } else {
            break;
        }
    }
}

QString GridWriterThread::process(QString text, QString win) {
    return highlighter->highlight(alter->addLink(alter->substitute(text, win), win));
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
    this->requestInterruption();
    dataQueue.stop();
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
    delete highlighter;
    delete alter;
}
