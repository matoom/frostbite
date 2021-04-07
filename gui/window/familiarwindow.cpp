#include "familiarwindow.h"

#include "mainwindow.h"
#include "windowfacade.h"
#include "genericwindow.h"
#include "windowwriterthread.h"
#include "defaultvalues.h"
#include "genericwindowfactory.h"

FamiliarWindow::FamiliarWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_FAMILIAR);
    window = (GenericWindow*)dock->widget();
    window->setStream(true);

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void FamiliarWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_FAMILIAR);
    this->visible = visible;
}

QDockWidget* FamiliarWindow::getDockWidget() {
    return dock;
}

void FamiliarWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_FAMILIAR : DOCK_TITLE_FAMILIAR " *");
    writer->addText(text);
    if(!writer->isRunning()) writer->start();
    if(!window->isVisible()) windowFacade->writeGameText(text.trimmed().toLocal8Bit(), false);
}

FamiliarWindow::~FamiliarWindow() {
    delete writer;
}
