#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkProxy>
#include <QDebug>
#include <session.h>

class ClientSettings;
class EAuthService;
class XmlParserThread;
class DebugLogger;
class Lich;
class Session;

class TcpClient : public QObject {
    Q_OBJECT

public:
    TcpClient(QObject *parent = 0, Lich* lichClient = 0, bool loadMock = false);
    ~TcpClient();
    void init();

    void writeCommand(QString);
    void logDebug(QByteArray buffer);
    void disconnectFromServer();

    void connectApi(QString host, QString port, QString user, QString password,
                    QString game, QString character, bool apiLich);

private:
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    ClientSettings *settings;
    EAuthService *eAuth;
    QString sessionKey;
    DebugLogger* debugLogger;
    QByteArray commandPrefix;
    Session* session;

    Lich* lich;

    void loadMockData();

    QString game;
    QString character;
    bool api;
    bool apiLich;
    bool isCmgr = false;
    bool useMock = false;
    
signals:
    void characterFound(QString, QString);
    void retrieveSessionKey(QString);
    void eAuthGameSelected(QString);
    void sessionRetrieved(QString, QString, QString);
    void eAuthError(QString);
    void addToQueue(QByteArray);
    void diconnected();
    void resetPassword();
    void enableGameSelect();
    void setGameList(QMap<QString, QString>);
    void connectAvailable(bool);
    void connectStarted();
    void connectSucceeded();
    void showMessage(QString);
public slots:
    void setProxy(bool, QString, QString);
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    bool connectToHost(QString host, QString port, QString sessionKey);
    void connectToHost(QString host, QString port);
    void connectToLocalPort(QString port);
    void connectToLich(QString sessionHost, QString sessionPort, QString sessionKey);
    void disconnectedFromHost();
    void connectedToHost();
    void initEauthSession(QString, QString, QString, QString);
    void selectGame(QMap<QString, QString>);
    void gameSelected(QString);
    void resetEauthSession();
    void addCharacter(QString, QString);
    void retrieveEauthSession(QString);
    void eAuthSessionRetrieved(QString, QString, QString);
    void connectWizardError(QString);
    void connectionWarning(QString warnMsg);
    void authError();
    void writeSettings();
    void writeDefaultSettings(QString);
    void writeModeSettings();
    void setGameModeCmgr(bool);
};


#endif // TCPCLIENT_H
