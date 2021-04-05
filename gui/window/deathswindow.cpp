#include "deathswindow.h"
#include "mainwindow.h"
#include "windowfacade.h"
#include "genericwindow.h"
#include "deathslogger.h"
#include "clientsettings.h"
#include "genericwindowfactory.h"
#include "windowwriterthread.h"
#include "defaultvalues.h"

DeathsWindow::DeathsWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_DEATHS);
    window = (GenericWindow*)dock->widget();

    writer = new WindowWriterThread(mainWindow, window);
    deathsLogger = new DeathsLogger();
    clientSettings = ClientSettings::getInstance();

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void DeathsWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_DEATHS);
    this->visible = visible;
}

QDockWidget* DeathsWindow::getDockWidget() {
    return dock;
}

void DeathsWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_DEATHS : DOCK_TITLE_DEATHS " *");

    writer->addText(text.trimmed());
    if(!writer->isRunning()) writer->start();

    this->log(text);
}

void DeathsWindow::log(QString text) {
    if(clientSettings->getParameter("Logging/deaths", false).toBool()) {
        deathsLogger->addText(text.trimmed());
        if(!deathsLogger->isRunning()) deathsLogger->start();
    }
}

DeathsWindow::~DeathsWindow() {
    delete deathsLogger;
    delete writer;
}
