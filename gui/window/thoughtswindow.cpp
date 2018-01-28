#include "thoughtswindow.h"

ThoughtsWindow::ThoughtsWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_THOUGHTS);
    window = (GenericWindow*)dock->widget();

    writer = new WindowWriterThread(mainWindow, window);
    thoughtsLogger = new ThoughtsLogger();
    clientSettings = ClientSettings::getInstance();

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void ThoughtsWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_THOUGHTS);
    this->visible = visible;
}

QDockWidget* ThoughtsWindow::getDockWidget() {
    return dock;
}

void ThoughtsWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_THOUGHTS : DOCK_TITLE_THOUGHTS " *");

    writer->addText(text.trimmed());
    if(!writer->isRunning()) writer->start();

    this->log(text);
}

void ThoughtsWindow::log(QString text) {
    if(clientSettings->getParameter("Logging/thoughts", false).toBool()) {
        thoughtsLogger->addText(text.trimmed());
        if(!thoughtsLogger->isRunning()) thoughtsLogger->start();
    }
}

ThoughtsWindow::~ThoughtsWindow() {
    delete thoughtsLogger;
    delete writer;
}
