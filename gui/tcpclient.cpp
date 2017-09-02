#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket(this);
    eAuth = new EAuthService(this);
    mainWindow = (MainWindow*)parent;
    windowFacade = mainWindow->getWindowFacade();
    settings = ClientSettings::getInstance();
    api = false;

    debugLogger = new DebugLogger();

    lich = new Lich(mainWindow);

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectedFromHost()));
        tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    }    

    connect(mainWindow, SIGNAL(profileChanged()), this, SLOT(reloadSettings()));

    xmlParser = new XmlParserThread(parent);        
    connect(this, SIGNAL(addToQueue(QByteArray)), xmlParser, SLOT(addData(QByteArray)));
    connect(this, SIGNAL(diconnected()), xmlParser, SLOT(flushStream()));
    connect(this, SIGNAL(updateHighlighterSettings()), xmlParser, SLOT(updateHighlighterSettings()));
    connect(xmlParser, SIGNAL(writeSettings()), this, SLOT(writeSettings()));
    connect(xmlParser, SIGNAL(writeModeSettings()), this, SLOT(writeModeSettings()));
    connect(xmlParser, SIGNAL(writeDefaultSettings(QString)), this, SLOT(writeDefaultSettings(QString)));

    if(MainWindow::DEBUG) {
        this->loadMockData();
    }
}

void TcpClient::reloadSettings() {
    emit updateHighlighterSettings();
}

void TcpClient::loadMockData() {
    QFile file(MOCK_DATA_PATH);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open mock data file!";
        return;
    }

    QByteArray mockData = file.readAll();

    emit addToQueue(mockData);

    if(!xmlParser->isRunning()) {
        xmlParser->start();
    }
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
        this->connectToHost(host, port, sessionKey);
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

void TcpClient::connectApi(QString host, QString port, QString user, QString password, QString game, QString character) {
    this->api = true;
    this->game = game;
    this->character = character;
    this->initEauthSession(host, port, user, password);
}

void TcpClient::connectToLich(QString sessionHost, QString sessionPort, QString sessionKey) {
    if(lich->isRunning()) lich->killLich();
    lich->run(sessionHost, sessionPort);
    lich->waitUntilRunning();
    sessionHost = "127.0.0.1";

    QTimer::singleShot(2000, [=] () {connectToHost(sessionHost, sessionPort, sessionKey);});
}

bool TcpClient::connectToHost(QString sessionHost, QString sessionPort, QString sessionKey) {
    this->api = false;

    windowFacade->writeGameWindow("Connecting ...");

    mainWindow->connectEnabled(false);

    tcpSocket->connectToHost(sessionHost, sessionPort.toInt());
    bool conntected = tcpSocket->waitForConnected();

    this->writeCommand(sessionKey);
    this->writeCommand("/FE:STORMFRONT /VERSION:1.0.1.26 /P:WIN_UNKNOWN /XML");

    return conntected;
}

void TcpClient::disconnectedFromHost() {
    mainWindow->connectEnabled(true);
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
    this->writeCommand("_swclose sassess");
    this->writeCommand("_swclose satmospherics");
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

    if(buffer.endsWith("\n") || xmlParser->isCmgr()) {
        // process raw data
        emit addToQueue(buffer);
        if(!xmlParser->isRunning()) {
            xmlParser->start();
        }
        buffer.clear();
    }
}

void TcpClient::writeCommand(QString cmd) {    
    QByteArray sendCmd = "<c>" + cmd.append("\n").toUtf8();
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
    mainWindow->connectEnabled(true);

    qDebug() << error;
}

void TcpClient::showError(QString message) {
    windowFacade->writeGameWindow("<br><br>"
        "*<br>"
        "* " + message.toLocal8Bit() + "<br>"
        "*<br>"
        "<br><br>");
}

void TcpClient::logDebug(QByteArray buffer) {
    if(settings->getParameter("Logging/debug", false).toBool()) {
        debugLogger->addText(buffer);
        if(!debugLogger->isRunning()) {
            debugLogger->start();
        }
    }
}

void TcpClient::disconnectFromServer() {
    if(tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        showError("Disconnected from server.");
        this->writeCommand("quit");
    }
    tcpSocket->disconnectFromHost();
    mainWindow->connectEnabled(true);
    emit diconnected();
}

TcpClient::~TcpClient() {
    this->disconnectFromServer();

    delete settings;
    delete debugLogger;
    delete tcpSocket;
    delete xmlParser;
    delete eAuth;
    delete lich;
}
