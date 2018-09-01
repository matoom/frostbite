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
#include <lich/lich.h>

#include <environment.h>

class MainWindow;
class WindowFacade;
class ClientSettings;
class EAuthService;
class XmlParserThread;
class DebugLogger;
class Lich;

class TcpClient : public QObject {
    Q_OBJECT

public:
    TcpClient(QObject *parent = 0);
    ~TcpClient();

    void writeCommand(QString);
    void showError(QString);
    void logDebug(QByteArray buffer);
    void disconnectFromServer();

    void connectApi(QString host, QString port, QString user, QString password,
                    QString game, QString character, bool apiLich);

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

    Lich* lich;

    void loadMockData();

    QString game;
    QString character;
    bool api;
    bool apiLich;

signals:
    void characterFound(QString, QString);
    void retrieveSessionKey(QString);
    void eAuthGameSelected(QString);
    void sessionRetrieved(QString, QString, QString);
    void eAuthError(QString);
    void addToQueue(QByteArray);
    void diconnected();
    void updateHighlighterSettings();
    void resetPassword();
    void enableGameSelect();
    void setGameList(QMap<QString, QString>);

public slots:
    void setProxy(bool, QString, QString);
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    bool connectToHost(QString, QString, QString);
    void connectToLich(QString sessionHost, QString sessionPort, QString sessionKey);
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
    void writeDefaultSettings(QString);
    void writeModeSettings();
    void reloadSettings();
};


#endif // TCPCLIENT_H
