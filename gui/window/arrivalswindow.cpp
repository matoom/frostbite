#include "arrivalswindow.h"

ArrivalsWindow::ArrivalsWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_ARRIVALS);
    window = (GenericWindow*)dock->widget();

    writer = new WindowWriterThread(mainWindow, window);
    arrivalsLogger = new ArrivalsLogger();
    clientSettings = ClientSettings::getInstance();

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void ArrivalsWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_ARRIVALS);
    this->visible = visible;
}

QDockWidget* ArrivalsWindow::getDockWidget() {
    return dock;
}

void ArrivalsWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_ARRIVALS : DOCK_TITLE_ARRIVALS " *");

    writer->addText(text.trimmed());
    if(!writer->isRunning()) writer->start();

    this->log(text);
}

void ArrivalsWindow::log(QString text) {
    if(clientSettings->getParameter("Logging/arrivals", false).toBool()) {
        arrivalsLogger->addText(text.trimmed());
        if(!arrivalsLogger->isRunning()) arrivalsLogger->start();
    }
}

ArrivalsWindow::~ArrivalsWindow() {
    delete arrivalsLogger;
    delete writer;
}
