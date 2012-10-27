#include "menuhandler.h"

MenuHandler::MenuHandler(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    connectDialog = new ConnectDialog(qobject_cast<QWidget *>(parent));
    highlightDialog = new HighlightDialog(qobject_cast<QWidget *>(parent));
    macroDialog = new MacroDialog(qobject_cast<QWidget *>(parent));
    appearanceDialog = new AppearanceDialog(qobject_cast<QWidget *>(parent));
}

void MenuHandler::menuTriggered(QAction* action) {
    if(action->text() == "Connect") {
        connectDialog->show();
    } else if (action->text() == "Disconnect") {
        mainWindow->getCommandLine()->writeCommand("quit");
    } else if(action->text() == "Highlight") {
        highlightDialog->show();
    } else if (action->text() == "Macros") {
        macroDialog->show();
    } else if (action->text() == "Appearance") {
        appearanceDialog->show();
    } else if(action->text() == "Exit") {
        mainWindow->close();
    }

    qDebug() << action->text();
}

MenuHandler::~MenuHandler() {
    delete appearanceDialog;
    delete connectDialog;
    delete highlightDialog;
}

