#include "groupwindow.h"

GroupWindow::GroupWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_GROUP);
    window = (GenericWindow*)dock->widget();
    window->setAppend(false);

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

QDockWidget* GroupWindow::getDockWidget() {
    return dock;
}

void GroupWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_GROUP);
    this->visible = visible;
}

void GroupWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_GROUP : DOCK_TITLE_GROUP " *");

    writer->addText(text);
    if(!writer->isRunning()) writer->start();
}

GroupWindow::~GroupWindow() {
    delete writer;
}
