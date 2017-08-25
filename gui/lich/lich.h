#ifndef LICH_H
#define LICH_H

#include <QObject>
#include <QProcess>
#include <QMutex>
#include <QReadLocker>

#include <clientsettings.h>
#include <mainwindow.h>

class MainWindow;

class Lich : public QObject {
    Q_OBJECT

public:
    explicit Lich(QObject *parent = 0);
    ~Lich();

    void run(QString host, QString port);

    bool isRunning();
    void waitUntilRunning();
    void killLich();

    void sendMessage(QByteArray message);

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;

    QProcess* lich_proc;
    ClientSettings* clientSettings;

    QMutex procMutex;
    QReadWriteLock lock;

signals:

public slots:
    void displayOutputMsg();
    void displayErrorMsg();
    void start();
    void finish(int exit);
    void handleError(QProcess::ProcessError error);
};

#endif // LICH_H
