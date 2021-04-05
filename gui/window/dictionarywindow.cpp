#include "dictionarywindow.h"

#include "mainwindow.h"
#include "windowfacade.h"
#include "genericwindow.h"
#include "windowwriterthread.h"
#include "defaultvalues.h"
#include "genericwindowfactory.h"

DictionaryWindow::DictionaryWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_DICTIONARY);
    window = (GenericWindow*)dock->widget();
    window->setAppend(false);

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void DictionaryWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_DICTIONARY);
    this->visible = visible;
}

QDockWidget* DictionaryWindow::getDockWidget() {
    return dock;
}

void DictionaryWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_DICTIONARY : DOCK_TITLE_DICTIONARY " *");
    writer->addText(text);
    if(!writer->isRunning()) writer->start();
}

DictionaryWindow::~DictionaryWindow() {
    delete writer;
}
