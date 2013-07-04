#include "menuhandler.h"

MenuHandler::MenuHandler(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    clientSettings = ClientSettings::Instance();

    connectWizard = new ConnectWizard(qobject_cast<QWidget *>(parent));
    highlightDialog = new HighlightDialog(qobject_cast<QWidget *>(parent));
    macroDialog = new MacroDialog(qobject_cast<QWidget *>(parent));
    appearanceDialog = new AppearanceDialog(qobject_cast<QWidget *>(parent));
    aboutDialog = new AboutDialog(qobject_cast<QWidget *>(parent));
    scriptEditDialog = new ScriptEditDialog(qobject_cast<QWidget *>(parent));
    profileAddDialog = new ProfileAddDialog();

    this->loadLoggingMenu();
}

void MenuHandler::openConnectDialog() {
    connectWizard->show();
}

void MenuHandler::updateDialogSettings() {
    highlightDialog->updateSettings();
    highlightDialog->loadSettings();

    appearanceDialog->updateSettings();
    appearanceDialog->loadSettings();

    macroDialog->updateSettings();
    macroDialog->clearMacros();
    macroDialog->loadSettings();
    macroDialog->loadSequenceTime();
}

void MenuHandler::menuTriggered(QAction* action) {
    if(action->text() == "Connect") {
        connectWizard->show();
    } else if (action->text() == "Disconnect") {
        mainWindow->getConnectionManager()->disconnectFromServer();
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
    } else if(action->text() == "Create new profile") {
        profileAddDialog->show();
    } else if(action->text() == "User Guide") {
        QDesktopServices::openUrl(QUrl("file:///" +
            QDir::currentPath() + "/docs/index.html", QUrl::TolerantMode));
    } else if(action->objectName() == "actionLogMain") {
        clientSettings->setParameter("Logging/main", action->isChecked());
    } else if(action->objectName() == "actionLogThoughts") {
        clientSettings->setParameter("Logging/thoughts", action->isChecked());
    } else if(action->objectName() == "actionLogConversations") {
        clientSettings->setParameter("Logging/conversations", action->isChecked());
    } else if(action->objectName() == "actionLogArrivals") {
        clientSettings->setParameter("Logging/arrivals", action->isChecked());
    } else if(action->objectName() == "actionLogDeaths") {
        clientSettings->setParameter("Logging/deaths", action->isChecked());
    } else if(action->objectName() == "actionLogDebug") {
        clientSettings->setParameter("Logging/debug", action->isChecked());
    }

    if(action->data() == "profile") {        
        if(action->isChecked()) {
            clientSettings->setParameter("Profile/name", action->text());
        } else {
            clientSettings->setParameter("Profile/name", "");
        }
        mainWindow->updateProfileSettings();
    }

    //qDebug() << action->text();
    //qDebug() << action->objectName();
}

void MenuHandler::menuHovered(QAction* action) {
    if(action->text() == "Profile") {
        this->loadProfilesMenu();
    }
}

void MenuHandler::loadProfilesMenu() {
    QDir dir(QDir::currentPath() + "/profiles");

    QStringList dirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

    profilesMenu = new QMenu();

    QString currentProfile = clientSettings->getParameter("Profile/name", "").toString();

    foreach(QString profile, dirList) {
        action = new QAction(profile, profilesMenu);
        action->setCheckable(true);
        action->setData("profile");

        if(currentProfile == profile) {
            action->setChecked(true);
        }
        profilesMenu->addAction(action);
    }
    mainWindow->insertProfilesMenu(profilesMenu);
}

void MenuHandler::loadLoggingMenu() {
    mainWindow->setLogMain(clientSettings->getParameter("Logging/main", false).toBool());
    mainWindow->setLogThoughts(clientSettings->getParameter("Logging/thoughts", false).toBool());
    mainWindow->setLogConversations(clientSettings->getParameter("Logging/conversations", false).toBool());
    mainWindow->setLogArrivals(clientSettings->getParameter("Logging/arrivals", false).toBool());
    mainWindow->setLogDeaths(clientSettings->getParameter("Logging/deaths", false).toBool());
    mainWindow->setLogDebug(clientSettings->getParameter("Logging/debug", false).toBool());
}

MenuHandler::~MenuHandler() {
    delete appearanceDialog;
    delete highlightDialog;
    delete macroDialog;
    delete aboutDialog;
    delete scriptEditDialog;
    delete profileAddDialog;
}

