#ifndef TRAY_H
#define TRAY_H

#include <QObject>

#include <QMenu>
#include <QSystemTrayIcon>

#include <mainwindow.h>

class MainWindow;

class Tray : public QObject {
    Q_OBJECT

public:
    Tray(QObject *parent = 0);

private:
    MainWindow* mainWindow;

    void createTrayIcon();
    void createActions();

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *exitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QRegExp rxRemoveTags;

public slots:
    void showMessage(QString, QString);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);

};

#endif // TRAY_H
