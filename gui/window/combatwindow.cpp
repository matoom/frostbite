#include "combatwindow.h"

#include "mainwindow.h"
#include "windowfacade.h"
#include "genericwindow.h"
#include "windowwriterthread.h"
#include "defaultvalues.h"
#include "genericwindowfactory.h"

CombatWindow::CombatWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_COMBAT);
    window = (GenericWindow*)dock->widget();
    window->setStream(true);

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void CombatWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_COMBAT);
    this->visible = visible;
}

QDockWidget* CombatWindow::getDockWidget() {
    return dock;
}

void CombatWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_COMBAT : DOCK_TITLE_COMBAT " *");
    writer->addText(text);
    if(!writer->isRunning()) writer->start();
    if(!window->isVisible()) windowFacade->writeGameText(text.trimmed().toLocal8Bit(), false);
}

CombatWindow::~CombatWindow() {
    delete writer;
}
