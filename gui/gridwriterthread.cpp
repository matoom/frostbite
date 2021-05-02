#include "gridwriterthread.h"
#include "mainwindow.h"
#include "gridwindow.h"
#include "text/alter/alter.h"
#include "text/highlight/highlighter.h"

GridWriterThread::GridWriterThread(QObject *parent, GridWindow* window) {
    mainWindow = (MainWindow*)parent;
    highlighter = new Highlighter(parent);
    alter = new Alter();
    connect(this, &GridWriterThread::finished, alter, &QObject::deleteLater);
    this->window = window;
}

void GridWriterThread::updateSettings() {
    highlighter->reloadSettings();
    alter->reloadSettings();
}

void GridWriterThread::addItem(QString name, QString text) {
    Parent::addData({name, text});
}

QString GridWriterThread::process(QString text, QString win) {
    return highlighter->highlight(alter->addLink(alter->substitute(text, win), win));
}

void GridWriterThread::onProcess(const GridEntry& gridEntry) {
    if(alter->ignore(gridEntry.text, window->objectName())) return;

    if(gridEntry.text.isEmpty()) {
        highlightedItems.remove(gridEntry.name);
    } else {
        highlightedItems.insert(gridEntry.name, this->process(gridEntry.text, window->objectName()));
    }
    emit writeGrid(highlightedItems);
}
