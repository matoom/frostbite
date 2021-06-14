#include "tcpclient.h"

#include <QTimer>
#include <QTime>

#include "clientsettings.h"
#include "eauthservice.h"
#include "debuglogger.h"
#include "lich/lich.h"
#include "environment.h"

TcpClient::TcpClient(QObject *parent, bool loadMock) : QObject(parent) {
    tcpSocket = new QTcpSocket(this);
    eAuth = new EAuthService(this);
    // TODO: Remove dependency on ClientSettings
    // settings only needed to get the current debug flag.
    // Probably emit a message instead ?
    settings = ClientSettings::getInstance();
    api = false;
    apiLich = false;
    useMock = loadMock;

    commandPrefix = "<c>";

    debugLogger = new DebugLogger();

    // TODO: We can use dependency injection here
    lich = new Lich(parent);

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectedFromHost()));
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(connectedToHost()));
        tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    }    
}

void TcpClient::init() {
    if(useMock) {
        this->loadMockData();
    }
}

void TcpClient::loadMockData() {
    QFile file(MOCK_DATA_PATH);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open mock data file!";
        return;
    }

    QByteArray mockData = file.readAll();

    emit addToQueue(mockData);
}

void TcpClient::initEauthSession(QString host, QString port, QString user, QString password) {
    eAuth->init(user, password);
    eAuth->initSession(host, port);
}

void TcpClient::selectGame(QMap<QString, QString> gameList) {
    if(api) {
        this->gameSelected(this->game);
    } else {
        // eAuth -> connectWizard
        emit setGameList(gameList);
        emit enableGameSelect();
    }
}

void TcpClient::gameSelected(QString id) {
    // connectWizard -> eAuth
    emit eAuthGameSelected(id);
}

void TcpClient::resetEauthSession() {
    eAuth->resetSession();
}

void TcpClient::addCharacter(QString id, QString name) {
    if(api) {
        if(this->character == name) {
            this->retrieveEauthSession(id);
        } else {
            this->connectWizardError("Character not found.");
            this->resetEauthSession();
        }
    } else {
        emit characterFound(id, name);
    }
}

void TcpClient::retrieveEauthSession(QString id) {
    emit retrieveSessionKey(id);
}

void TcpClient::eAuthSessionRetrieved(QString host, QString port, QString sessionKey) {
    if(api) {
        if(apiLich) {
            this->connectToLich(host, port, sessionKey);
        } else {
            this->connectToHost(host, port, sessionKey);
        }
    } else {
        emit sessionRetrieved(host, port, sessionKey);
    }
}

void TcpClient::connectWizardError(QString errorMsg) {
    this->api = false;
    emit eAuthError(errorMsg);
    this->showError(errorMsg);
}

void TcpClient::authError() {
    this->api = false;
    emit resetPassword();    
}

void TcpClient::connectApi(QString host, QString port, QString user, QString password,
        QString game, QString character, bool apiLich) {
    this->api = true;
    this->game = game;
    this->character = character;
    this->apiLich = apiLich;
    this->initEauthSession(host, port, user, password);
}

void TcpClient::connectToLich(QString sessionHost, QString sessionPort, QString sessionKey) {
    if(lich->isRunning()) lich->killLich();
    lich->run(sessionHost, sessionPort);
    lich->waitUntilRunning();
    sessionHost = "127.0.0.1";

    QTimer::singleShot(3000, [=] () {connectToHost(sessionHost, sessionPort, sessionKey);});
}

void TcpClient::connectToLocalPort(QString port) {
    this->connectToHost("127.0.0.1", port);
}

void TcpClient::connectToHost(QString host, QString port) {
    emit connectAvailable(false);
    this->api = false;
    emit connectStarted();
    commandPrefix = "";

    tcpSocket->connectToHost(host, port.toInt());
}

bool TcpClient::connectToHost(QString sessionHost, QString sessionPort, QString sessionKey) {
    emit connectAvailable(false);    
    emit connectStarted();
    this->api = false;
    commandPrefix = "<c>";

    tcpSocket->connectToHost(sessionHost, sessionPort.toInt());
    bool connected = tcpSocket->waitForConnected();

    this->writeCommand(sessionKey);
    this->writeCommand("/FE:STORMFRONT /VERSION:1.0.1.26 /P:WIN_UNKNOWN /XML");

    return connected;
}

void TcpClient::disconnectedFromHost() {
    emit connectAvailable(true);
}

void TcpClient::connectedToHost() {
    emit connectSucceeded();
}

void TcpClient::setProxy(bool enabled, QString proxyHost, QString proxyPort) {
    if(enabled) {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(proxyHost);
        proxy.setPort(proxyPort.toInt());

        QNetworkProxy::setApplicationProxy(proxy);
    } else {
        QNetworkProxy proxy(QNetworkProxy::NoProxy);
        QNetworkProxy::setApplicationProxy(proxy);
    }
}

void TcpClient::writeModeSettings() {
    this->writeCommand("");
    this->writeCommand("_STATE CHATMODE OFF");
}

// TODO: does this have any effect?
void TcpClient::writeSettings() {
    this->writeCommand("");
    this->writeCommand("_swclose sooc");
}

void TcpClient::writeDefaultSettings(QString settings) {
    this->writeCommand("<db>" + settings);
}

void TcpClient::socketReadyRead() {
    QByteArray data = tcpSocket->readAll();

    // log raw data
    this->logDebug(data);

    buffer.append(data);
    if(buffer.endsWith("\n") || isCmgr) {
        // process raw data
        emit addToQueue(buffer);
        buffer.clear();
    }
}

void TcpClient::writeCommand(QString cmd) {
    QByteArray sendCmd = commandPrefix + cmd.append("\r\n").toUtf8();
    this->logDebug(sendCmd);
    tcpSocket->write(sendCmd);
    tcpSocket->flush();
}

void TcpClient::socketError(QAbstractSocket::SocketError error) {
    if(error == QAbstractSocket::RemoteHostClosedError) {        
        this->showError("Disconnected from server. [" + QTime::currentTime().toString("h:mm ap") + "]");
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        this->showError("Unable to connect to server. Please check your internet connection "
                        "and try again later. [" + QTime::currentTime().toString("h:mm ap") + "]");
    } else if (error == QAbstractSocket::NetworkError) {
        this->showError("Connection timed out. [" + QTime::currentTime().toString("h:mm ap") + "]");
    } else if (error == QAbstractSocket::HostNotFoundError) {
        this->showError("Unable to resolve game host. [" + QTime::currentTime().toString("h:mm ap") + "]");
    }    
    emit connectAvailable(true);

    qDebug() << error;
}

void TcpClient::showError(QString message) {
    emit connectFailed(message);
}

void TcpClient::logDebug(QByteArray buffer) {
    if(settings->getParameter("Logging/debug", false).toBool()) {
        debugLogger->addText(buffer);
        if(!debugLogger->isRunning()) {
            debugLogger->start();
        }
    }
}

void TcpClient::setGameModeCmgr(bool cmgr) {
    isCmgr = cmgr;
}

void TcpClient::disconnectFromServer() {
    if(tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        showError("Disconnected from server.");
        this->writeCommand("quit");
    }
    tcpSocket->disconnectFromHost();
    emit connectAvailable(true);
    emit diconnected();
}

TcpClient::~TcpClient() {
    this->disconnectFromServer();

    delete debugLogger;
    delete tcpSocket;
    delete eAuth;
    delete lich;
}

