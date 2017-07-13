#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkProxy>
#include <QDebug>

#include <mainwindow.h>
#include <windowfacade.h>
#include <clientsettings.h>
#include <eauthservice.h>
#include <xml/xmlparserthread.h>
#include <debuglogger.h>

#include <environment.h>

class MainWindow;
class WindowFacade;
class ClientSettings;
class EAuthService;
class XmlParserThread;
class DebugLogger;

class TcpClient : public QObject {
    Q_OBJECT

public:
    TcpClient(QObject *parent = 0);
    ~TcpClient();

    void writeCommand(QString);
    void showError(QString);
    void log(QByteArray buffer);
    void updateSettings();
    void disconnectFromServer();

    void connectApi(QString host, QString port, QString user, QString password, QString game, QString character);

private:
    MainWindow *mainWindow;
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    WindowFacade *windowFacade;
    ClientSettings *settings;
    EAuthService *eAuth;
    QString sessionKey;
    XmlParserThread* xmlParser;
    DebugLogger* debugLogger;

    void loadMockData();

    QString game;
    QString character;
    bool api;

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
    void setGameList(QMap<QString, QString>);

public slots:
    void setProxy(bool, QString, QString);
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void connectToHost(QString, QString, QString);
    void disconnectedFromHost();
    void initEauthSession(QString, QString, QString, QString);
    void selectGame(QMap<QString, QString>);
    void gameSelected(QString);
    void resetEauthSession();
    void addCharacter(QString, QString);
    void retrieveEauthSession(QString);
    void eAuthSessionRetrieved(QString, QString, QString);
    void connectWizardError(QString);
    void authError();
    void writeSettings();
    void writeModeSettings();
};


#endif // TCPCLIENT_H
