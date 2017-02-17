#include "tray.h"

Tray::Tray(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    settings = new ClientSettings();
    conversations = settings->getParameter("Tray/conversations", true).toBool();

    createTrayIcon();

    connect(trayIcon, SIGNAL(messageClicked()), mainWindow, SLOT(showMaximized()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

void Tray::createTrayIcon() {    
    trayIcon = new QSystemTrayIcon(mainWindow);

    trayIconMenu = new QMenu(mainWindow);
    trayIcon->setContextMenu(trayIconMenu);

    trayIcon->setIcon(QIcon(":/window/images/shield.png"));

    this->createActions();

    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(conversationsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAction);
}

void Tray::createActions() {
     minimizeAction = new QAction("Minimize", mainWindow);
     connect(minimizeAction, SIGNAL(triggered()), mainWindow, SLOT(hide()));

     maximizeAction = new QAction("Maximize", mainWindow);
     connect(maximizeAction, SIGNAL(triggered()), mainWindow, SLOT(showMaximized()));

     restoreAction = new QAction("Restore", mainWindow);
     connect(restoreAction, SIGNAL(triggered()), mainWindow, SLOT(showNormal()));

     exitAction = new QAction("Exit", mainWindow);
     connect(exitAction, SIGNAL(triggered()), mainWindow, SLOT(close()));

     conversationsAction = new QAction("Conversations", mainWindow);
     conversationsAction->setCheckable(true);
     conversationsAction->setChecked(conversations);
     connect(conversationsAction, SIGNAL(changed()), this, SLOT(conversationsChanged()));
}

void Tray::conversationsChanged() {
    conversations = conversationsAction->isChecked();
    settings->setParameter("Tray/conversations", conversations);
}

void Tray::showMessage(QString title, QString trayEntry) {
    TextUtils::htmlToPlain(trayEntry);
    if(title == DOCK_TITLE_CONVERSATIONS) {
        if(conversations) {
            trayIcon->showMessage(title, trayEntry, QSystemTrayIcon::NoIcon, 3 * 1000);
        }
    } else {
        trayIcon->showMessage(title, trayEntry, QSystemTrayIcon::NoIcon, 5 * 1000);
    }
}

void Tray::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        mainWindow->showMaximized();
        break;
    case QSystemTrayIcon::MiddleClick:
    default:
        ;
    }
}

Tray::~Tray() {
    delete settings;
}
