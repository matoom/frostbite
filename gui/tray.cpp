#include "tray.h"

Tray::Tray(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(messageClicked()), mainWindow, SLOT(showMaximized()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

void Tray::createTrayIcon() {
    trayIconMenu = new QMenu(mainWindow);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAction);

    trayIcon = new QSystemTrayIcon(mainWindow);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon icon = QIcon(":/window/images/shield.png");
    trayIcon->setIcon(icon);    
}

void Tray::createActions() {
     minimizeAction = new QAction(tr("Mi&nimize"), mainWindow);
     connect(minimizeAction, SIGNAL(triggered()), mainWindow, SLOT(hide()));

     maximizeAction = new QAction(tr("Ma&ximize"), mainWindow);
     connect(maximizeAction, SIGNAL(triggered()), mainWindow, SLOT(showMaximized()));

     restoreAction = new QAction(tr("&Restore"), mainWindow);
     connect(restoreAction, SIGNAL(triggered()), mainWindow, SLOT(showNormal()));

     exitAction = new QAction(tr("&Exit"), mainWindow);
     connect(exitAction, SIGNAL(triggered()), mainWindow, SLOT(close()));
 }

void Tray::showMessage(QString title, QString trayEntry) {
    TextUtils::Instance()->htmlToPlain(trayEntry);
    trayIcon->showMessage(title, trayEntry,
                          QSystemTrayIcon::NoIcon, 5 * 1000);
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
