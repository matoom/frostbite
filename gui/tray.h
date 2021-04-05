#ifndef TRAY_H
#define TRAY_H

#include <QObject>

#include <QMenu>
#include <QSystemTrayIcon>

class MainWindow;
class ClientSettings;

class Tray : public QObject {
    Q_OBJECT

public:
    Tray(QObject *parent = 0);
    ~Tray();

private:
    MainWindow* mainWindow;

    void createTrayIcon();
    void createActions();

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *exitAction;
    QAction *conversationsAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    ClientSettings* settings;

    bool conversations;

public slots:
    void showMessage(QString, QString);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void conversationsChanged();

};

#endif // TRAY_H
