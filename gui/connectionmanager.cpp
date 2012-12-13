#include "connectionmanager.h"

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket(this);
    eAuth = new EAuthService(this);
    mainWindow = (MainWindow*)parent;
    windowManager = mainWindow->getWindowManager();
    settings = ClientSettings::Instance();

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectedFromHost()));
        tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    }    

    debug = false;

    dataProcessThread = new DataProcessThread(parent);
    connect(this, SIGNAL(addData(QByteArray)), dataProcessThread, SLOT(addData(QByteArray)));

    if(debug) {
        QFile file(MOCK_DATA_PATH);

        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Unable to open mock data file!";
            return;
        }
        QByteArray data = file.readAll();

        dataProcessThread->addData(data);
        if(!dataProcessThread->isRunning()) {
            dataProcessThread->start();
        }
    }
}

void ConnectionManager::initEauthSession(QString user, QString password) {
    eAuth->init(user, password);
    eAuth->initSession();
}

void ConnectionManager::resetEauthSession() {
    eAuth->resetSession();
}

void ConnectionManager::addCharacter(QString id, QString name) {
    emit characterFound(id, name);
}

void ConnectionManager::retrieveEauthSessionKey(QString id) {
    emit retrieveSessionKey(id);
}

void ConnectionManager::eAuthsessionKeyRetrieved(QString sessionKey) {
    emit sessionKeyRetrieved(sessionKey);
}

void ConnectionManager::connectWizardError(QString errorMsg) {
    emit eAuthError(errorMsg);
}

void ConnectionManager::connectToHost(QString sessionKey) {
    waitStartCommand = true;

    mainWindow->connectEnabled(false);
    tcpSocket->connectToHost(settings->getParameter("Login/gameHost", "").toString(),
                             settings->getParameter("Login/gamePort", "").toInt());

    tcpSocket->write("<c>" + sessionKey.toLocal8Bit() + "\n" +
                     "<c>/FE:STORMFRONT /VERSION:1.0.1.26 /P:WIN_XP /XML\n");
}

void ConnectionManager::disconnectedFromHost() {
    mainWindow->connectEnabled(true);
}

/* proxy */
/*void ConnectionManager::socketReadyRead() {
    buffer.append(tcpSocket->readAll());
    qDebug() << buffer;
        if (buffer.endsWith("\r\n")) {
            if(buffer.contains("200 Connection established")) {
                qDebug() << "WRITE";
                tcpSocket->flush();
                tcpSocket->write("52f84c85522eac4df965bb570aad4993\r\n");
                tcpSocket->write("FE/JAVA\r\n");
            }
            buffer.clear();
        }
}
*/

void ConnectionManager::socketReadyRead() {    
    buffer.append(tcpSocket->readAll());

    if(buffer.endsWith("\n")){
        if(!debug) {
            if(waitStartCommand) {
                if(buffer.endsWith("instance='DR'/>\n")) {
                    this->writeCommand("");
                    this->writeCommand("_STATE CHATMODE OFF");
                    this->writeCommand("");
                    this->writeCommand("_swclose sassess");
                    waitStartCommand = false;

                    return;
                }
            }
            emit addData(buffer);
            if(!dataProcessThread->isRunning()) {
                dataProcessThread->start();
            }
        } else {
            windowManager->writeGameWindow(buffer.data());
        }
        buffer.clear();
    }
}

void ConnectionManager::writeCommand(QString cmd) {
    tcpSocket->write("<c>" + cmd.append("\n").toLocal8Bit());
    tcpSocket->flush();
}

void ConnectionManager::socketError(QAbstractSocket::SocketError error) {
    if(error == QAbstractSocket::RemoteHostClosedError) {
        this->showError("Disconnected from server.");
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        this->showError("Unable to connect to server. Please check your internet connection and try again later.");
    } else if (error == QAbstractSocket::NetworkError) {
        this->showError("Connection timed out.");
    } else if (error == QAbstractSocket::HostNotFoundError) {
        this->showError("Unable to resolve game host.");
        mainWindow->connectEnabled(true);
    }

    qDebug() << error;
}

void ConnectionManager::showError(QString message) {
    windowManager->writeGameWindow("<br><br>"
        "*<br>"
        "* " + message.toLocal8Bit() + "<br>"
        "*<br>"
        "<br><br>");
}

ConnectionManager::~ConnectionManager() {
    tcpSocket->disconnectFromHost();

    delete tcpSocket;
}
