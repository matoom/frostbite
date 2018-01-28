#include "conversationswindow.h"

ConversationsWindow::ConversationsWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    visible = false;

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_CONVERSATIONS);
    window = (GenericWindow*)dock->widget();

    writer = new WindowWriterThread(mainWindow, window);
    conversationsLogger = new ConversationsLogger();
    clientSettings = ClientSettings::getInstance();

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(setVisible(bool)));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

void ConversationsWindow::setVisible(bool visible) {
    dock->setWindowTitle(DOCK_TITLE_CONVERSATIONS);
    this->visible = visible;
}

QDockWidget* ConversationsWindow::getDockWidget() {
    return dock;
}

void ConversationsWindow::write(QString text) {
    dock->setWindowTitle(visible ? DOCK_TITLE_CONVERSATIONS : DOCK_TITLE_CONVERSATIONS " *");

    writer->addText(text.trimmed());
    if(!writer->isRunning()) writer->start();

    mainWindow->getTray()->showMessage(DOCK_TITLE_CONVERSATIONS, text.trimmed());
    this->log(text);
}

void ConversationsWindow::log(QString text) {
    if(clientSettings->getParameter("Logging/conversations", false).toBool()) {
        conversationsLogger->addText(text.trimmed());
        if(!conversationsLogger->isRunning()) conversationsLogger->start();
    }
}

ConversationsWindow::~ConversationsWindow() {
    delete writer;
}
