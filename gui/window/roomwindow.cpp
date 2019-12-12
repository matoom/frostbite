#include "roomwindow.h"

RoomWindow::RoomWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    gameDataContainer = GameDataContainer::Instance();

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_ROOM);
    window = (GenericWindow*)dock->widget();
    window->setAppend(false);

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

QDockWidget* RoomWindow::getDockWidget() {
    return dock;
}

void RoomWindow::write() {
    QString roomText = "";

    QString desc = gameDataContainer->getRoomDesc();
    roomText += desc.isEmpty() ? "" : desc + "\n";

    QString objs = gameDataContainer->getRoomObjsData();
    roomText += objs.isEmpty() ? "" : objs + "\n";

    QString players = gameDataContainer->getRoomPlayers();
    roomText += players.isEmpty() ? "" : players + "\n";

    QString exits = gameDataContainer->getRoomExits();
    roomText += exits.isEmpty() ? "" : exits + "\n";

    writer->addText(roomText);
    if(!writer->isRunning()) writer->start();
}

void RoomWindow::setTitle(QString title) {
    dock->setWindowTitle(DOCK_TITLE_ROOM + title);
}

RoomWindow::~RoomWindow() {
    delete writer;
}
