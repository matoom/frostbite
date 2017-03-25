#include "menuhandler.h"

MenuHandler::MenuHandler(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    clientSettings = new ClientSettings();

    connectWizard = new ConnectWizard(qobject_cast<QWidget *>(parent));
    highlightDialog = new HighlightDialog(qobject_cast<QWidget *>(parent));
    macroDialog = new MacroDialog(qobject_cast<QWidget *>(parent));
    alterDialog = new AlterDialog(qobject_cast<QWidget *>(parent));
    appearanceDialog = new AppearanceDialog(qobject_cast<QWidget *>(parent));
    aboutDialog = new AboutDialog(qobject_cast<QWidget *>(parent));
    scriptEditDialog = new ScriptEditDialog(qobject_cast<QWidget *>(parent));
    profileAddDialog = new ProfileAddDialog();

    connect(profileAddDialog, SIGNAL(updateMenu()), this, SLOT(loadProfilesMenu()));

    this->loadLoggingMenu();
    this->loadToolbarMenu();
}

void MenuHandler::openConnectDialog() {
    connectWizard->show();
}

void MenuHandler::openAppearanceDialog() {
    appearanceDialog->show();
}

void MenuHandler::updateDialogSettings() {
    alterDialog->updateSettings();

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
    if(action->objectName() == "actionConnect") {
        connectWizard->show();
    } else if (action->objectName() == "actionDisconnect") {
        mainWindow->getTcpClient()->disconnectFromServer();
    } else if(action->objectName() == "actionText_Highlight") {
        highlightDialog->show();
    } else if (action->objectName() == "actionMacros") {
        macroDialog->show();
    } else if (action->objectName() == "actionSubstitute") {
        alterDialog->show();
    } else if (action->objectName() == "actionAppearance") {
        appearanceDialog->show();
    } else if(action->objectName() == "actionExit") {
        mainWindow->close();
    } else if(action->objectName() == "actionAbout") {
        aboutDialog->show();
    } else if(action->objectName() == "actionEdit") {
        scriptEditDialog->show();        
    } else if(action->objectName() == "actionCreate_new_profile") {
        profileAddDialog->show();
    } else if(action->objectName() == "actionReference_Manual") {
        QDesktopServices::openUrl(QUrl(USER_GUIDE_URL));
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
    } else if(action->objectName() == "actionLogAuth") {
        clientSettings->setParameter("Logging/auth", action->isChecked());
    } else if(action->objectName() == "actionWindowSave") {
        mainWindow->saveWindow();
    } else if(action->objectName() == "actionToolWieldLeft") {
        clientSettings->setParameter("Toolbar/wieldLeft", action->isChecked());
        mainWindow->getToolbar()->setWieldLeftVisible(action->isChecked());
    } else if(action->objectName() == "actionToolWieldRight") {
        clientSettings->setParameter("Toolbar/wieldRight", action->isChecked());
        mainWindow->getToolbar()->setWieldRightVisible(action->isChecked());
    } else if(action->objectName() == "actionToolSpell") {
        clientSettings->setParameter("Toolbar/spell", action->isChecked());
        mainWindow->getToolbar()->setSpellVisible(action->isChecked());
    } else if(action->objectName() == "actionToolActiveSpells") {
        clientSettings->setParameter("Toolbar/activeSpells", action->isChecked());
        mainWindow->getToolbar()->setActiveSpellsVisible(action->isChecked());
    } else if(action->objectName() == "actionToolStatus") {
        clientSettings->setParameter("Toolbar/status", action->isChecked());
        mainWindow->getToolbar()->setStatusVisible(action->isChecked());
    } else if(action->objectName() == "actionToolButtons") {
        clientSettings->setParameter("Toolbar/buttons", action->isChecked());
        mainWindow->getToolbar()->setButtonsVisible(action->isChecked());
    } else if(action->objectName() == "actionToolVitals") {
        clientSettings->setParameter("Toolbar/vitals", action->isChecked());
        mainWindow->getToolbar()->setVitalsVisible(action->isChecked());
    }
}

void MenuHandler::menuHovered(QAction* action) {
    if(action->objectName() == "actionLoad_profile") {
        this->loadProfilesMenu();
    }
}

void MenuHandler::profileTriggered(QAction* action) {
    if(action->isChecked()) {
        mainWindow->updateProfileSettings(action->text());
    } else {
        mainWindow->updateProfileSettings("");
    }    
    this->loadProfilesMenu();
}

void MenuHandler::loadProfilesMenu() {
    QDir dir(QApplication::applicationDirPath()  + "/profiles");

    QStringList dirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

    profilesMenu = new QMenu(mainWindow);

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

    connect(profilesMenu, SIGNAL(triggered(QAction*)), this, SLOT(profileTriggered(QAction*)));
}

void MenuHandler::loadLoggingMenu() {
    mainWindow->setLogMain(clientSettings->getParameter("Logging/main", false).toBool());
    mainWindow->setLogThoughts(clientSettings->getParameter("Logging/thoughts", false).toBool());
    mainWindow->setLogConversations(clientSettings->getParameter("Logging/conversations", false).toBool());
    mainWindow->setLogArrivals(clientSettings->getParameter("Logging/arrivals", false).toBool());
    mainWindow->setLogDeaths(clientSettings->getParameter("Logging/deaths", false).toBool());
    mainWindow->setLogDebug(clientSettings->getParameter("Logging/debug", false).toBool());
    mainWindow->setLogAuth(clientSettings->getParameter("Logging/auth", false).toBool());
}

void MenuHandler::loadToolbarMenu() {
    mainWindow->setMenuWieldLeftVisible(clientSettings->getParameter("Toolbar/wieldLeft", true).toBool());
    mainWindow->setMenuWieldRightVisible(clientSettings->getParameter("Toolbar/wieldRight", true).toBool());
    mainWindow->setMenuSpellVisible(clientSettings->getParameter("Toolbar/spell", true).toBool());
    mainWindow->setMenuActiveSpellsVisible(clientSettings->getParameter("Toolbar/activeSpells", true).toBool());
    mainWindow->setMenuStatusVisible(clientSettings->getParameter("Toolbar/status", true).toBool());
    mainWindow->setMenuButtonsVisible(clientSettings->getParameter("Toolbar/buttons", true).toBool());
    mainWindow->setMenuVitalsVisible(clientSettings->getParameter("Toolbar/vitals", true).toBool());
}

MenuHandler::~MenuHandler() {
    delete clientSettings;
    delete appearanceDialog;
    delete highlightDialog;
    delete macroDialog;
    delete alterDialog;
    delete aboutDialog;
    delete scriptEditDialog;
    delete profileAddDialog;
    delete profilesMenu;
}

