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

    void writeCommand(QString);
    void showError(QString);

private:
    MainWindow *mainWindow;
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    CommandParser *commandParser;
    WindowManager *windowManager;
    ClientSettings *settings;
    EAuthService *eAuth;
    QString sessionKey;

    bool waitStartCommand;
    bool debug;
    bool est;        

signals:
    void characterFound(QString, QString);
    void retrieveSessionKey(QString);
    void sessionKeyRetrieved(QString);
    void eAuthError(QString);

public slots:
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void connectToHost(QString);
    void disconnectedFromHost();
    void initEauthSession(QString, QString);
    void resetEauthSession();
    void addCharacter(QString, QString);
    void retrieveEauthSessionKey(QString);
    void eAuthsessionKeyRetrieved(QString);
    void connectWizardError(QString);
};


#endif // CONNECTIONMANAGER_H
