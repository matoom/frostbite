#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkProxy>
#include <QDebug>

#include <mainwindow.h>
#include <windowmanager.h>
#include <clientsettings.h>
#include <eauthservice.h>
#include <dataprocessthread.h>
#include <debuglogger.h>

#include <environment.h>

class MainWindow;
class WindowManager;
class ClientSettings;
class EAuthService;
class DataProcessThread;
class DebugLogger;

class ConnectionManager : public QObject {
    Q_OBJECT

public:
    ConnectionManager(QObject *parent = 0);
    ~ConnectionManager();

    void writeCommand(QString);
    void showError(QString);
    void updateSettings();
    void disconnectFromServer();    

private:
    MainWindow *mainWindow;
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    WindowManager *windowManager;
    ClientSettings *settings;
    EAuthService *eAuth;
    QString sessionKey;
    DataProcessThread* dataProcessThread;
    DebugLogger* debugLogger;

    bool waitForSettings;

    void loadMockData();

signals:
    void characterFound(QString, QString);
    void retrieveSessionKey(QString);
    void eAuthGameSelected(QString);
    void sessionRetrieved(QString, QString, QString);
    void eAuthError(QString);
    void addToQueue(QByteArray);
    void updateHighlighterSettings();
    void resetPassword();
    void enableGameSelect();

public slots:
    void setProxy(bool, QString, QString);
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void connectToHost(QString, QString, QString);
    void disconnectedFromHost();
    void initEauthSession(QString, QString, QString, QString);
    void selectGame();
    void gameSelected(QString);
    void resetEauthSession();
    void addCharacter(QString, QString);
    void retrieveEauthSession(QString);
    void eAuthSessionRetrieved(QString, QString, QString);
    void connectWizardError(QString);
    void authError();
};


#endif // CONNECTIONMANAGER_H
