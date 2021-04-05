#include "atmosphericswindow.h"

#include "mainwindow.h"
#include "windowfacade.h"
#include "genericwindow.h"
#include "windowwriterthread.h"
#include "defaultvalues.h"
#include "genericwindowfactory.h"

AtmosphericsWindow::AtmosphericsWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_ATMOSPHERICS);
    window = (GenericWindow*)dock->widget();

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void AtmosphericsWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_ATMOSPHERICS);
    this->visible = visible;
}

QDockWidget* AtmosphericsWindow::getDockWidget() {
    return dock;
}

void AtmosphericsWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_ATMOSPHERICS : DOCK_TITLE_ATMOSPHERICS " *");
    writer->addText(text);
    if(!writer->isRunning()) writer->start();
    if(!window->isVisible()) windowFacade->writeGameText(text.trimmed().toLocal8Bit(), false);
}

AtmosphericsWindow::~AtmosphericsWindow() {
    delete writer;
}
