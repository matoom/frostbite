#include "menuhandler.h"

MenuHandler::MenuHandler(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    connectDialog = new ConnectDialog(qobject_cast<QWidget *>(parent));
    highlightDialog = new HighlightDialog(qobject_cast<QWidget *>(parent));
}

void MenuHandler::menuTriggered(QAction* action) {
    if(action->text() == "Connect") {
        connectDialog->show();
    } else if (action->text() == "Disconnect") {
        mainWindow->getCommandLine()->setText("quit");
        mainWindow->getCommandLine()->writeCommand();
    } else if(action->text() == "Highlight") {
        highlightDialog->show();
    } else if(action->text() == "Exit") {
        mainWindow->close();
    }

    qDebug() << action->text();
}

MenuHandler::~MenuHandler() {
    delete connectDialog;
    delete highlightDialog;
}

