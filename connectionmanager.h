#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkProxy>
#include <QDebug>

#include <mainwindow.h>
#include <commandparser.h>
#include <windowmanager.h>
#include <clientsettings.h>
#include <eauthservice.h>

#include <environment.h>

class MainWindow;
class WindowManager;
class CommandParser;
class ClientSettings;
class EAuthService;

class ConnectionManager : public QObject {
    Q_OBJECT

public:
    ConnectionManager(QObject *parent = 0);
    ~ConnectionManager();

    void initLoginSession(QString, QString);
    void writeCommand(QString);

public slots:
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void connectToHost(QString);

private:
    MainWindow *mainWindow;
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    CommandParser *commandParser;
    WindowManager *windowManager;
    ClientSettings *settings;
    EAuthService *eAuth;

    bool waitStartCommand;
    bool debug;
    bool est;
};


#endif // CONNECTIONMANAGER_H
