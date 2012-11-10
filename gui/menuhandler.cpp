#include "menuhandler.h"

MenuHandler::MenuHandler(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    connectWizard = new ConnectWizard(qobject_cast<QWidget *>(parent));
    highlightDialog = new HighlightDialog(qobject_cast<QWidget *>(parent));
    macroDialog = new MacroDialog(qobject_cast<QWidget *>(parent));
    appearanceDialog = new AppearanceDialog(qobject_cast<QWidget *>(parent));
    aboutDialog = new AboutDialog(qobject_cast<QWidget *>(parent));
    scriptEditDialog = new ScriptEditDialog(qobject_cast<QWidget *>(parent));
}

void MenuHandler::menuTriggered(QAction* action) {
    if(action->text() == "Connect") {
        connectWizard->show();
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
    } else if(action->text() == "About") {
        aboutDialog->show();
    } else if(action->text() == "Edit/Create") {
        scriptEditDialog->show();
    } else if(action->text() == "User Guide") {
        QDesktopServices::openUrl(QUrl("file:///" +
            QDir::currentPath() + "/docs/index.html", QUrl::TolerantMode));
    }

    qDebug() << action->text();
}

MenuHandler::~MenuHandler() {
    delete appearanceDialog;
    delete highlightDialog;
    delete macroDialog;
    delete aboutDialog;
    delete scriptEditDialog;
}

