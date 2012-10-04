#include "connectionmanager.h"

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket(this);
    eAuth = new EAuthService(this);

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    }

    mainWindow = (MainWindow*)parent;
    windowManager = mainWindow->getWindowManager();
    settings = ClientSettings::Instance();

    waitStartCommand = true;
    debug = true;

    commandParser = new CommandParser(parent);

    if(debug) {
        commandParser->processMock();
    }
}

void ConnectionManager::initLoginSession(QString user, QString key) {
    eAuth->init(user, key);
    eAuth->initiateSession();
}

void ConnectionManager::connectToHost(QString sessionKey) {
    if(settings->getParameter("Connection/useProxy", false).toBool()) {
        qDebug() << "USE PROXY";
        /*tcpSocket->connectToHost("192.168.1.68", 3128);
        tcpSocket->write("CONNECT prime.dr.game.play.net:4901 HTTP/1.1\r\n");
        tcpSocket->write("\r\n");*/
    } else {
        tcpSocket->connectToHost(settings->getParameter("Login/serverHost", "").toString(),
                                 settings->getParameter("Login/serverPort", "").toInt());
    }

    tcpSocket->write(sessionKey.toLocal8Bit() + "\r\n");
    tcpSocket->write("<c>/FE:STORMFRONT /VERSION:1.0.1.26 /P:WIN_XP /XML\r\n");

    /*<c>_STATE CHATMODE OFF
    <c>_swclose sconversation
    <c>_swclose swhispers
    <c>_swclose stalk
    <c>_swclose sexperience
    <c>_swclose sgroup
    <c>_swclose satmospherics
    <c>_swclose sooc*/
}

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

    if (buffer.endsWith("\r\n")){
        if(!debug) {
            if(waitStartCommand) {
                if(buffer.endsWith("GSw000100000150095\r\n")) {
                    this->writeCommand("<c>_STATE CHATMODE OFF\n\n");
                    waitStartCommand = false;
                    buffer.chop(22);
                    buffer.append("\r\n\r\n");

                    return;
                }
            }

            qDebug() << buffer;
            commandParser->process(buffer);
        } else {
            windowManager->writeGameWindow(buffer.data());
        }

        buffer.clear();
    }
}

void ConnectionManager::writeCommand(QString cmd) {
    tcpSocket->write(cmd.append("\n").toLocal8Bit());
}

void ConnectionManager::socketError(QAbstractSocket::SocketError error) {
    if(error == QAbstractSocket::RemoteHostClosedError) {
        //QMessageBox::information(mainWindow, tr("Message"), tr("Disconnected from server."), QMessageBox::Ok, 0);

        windowManager->writeGameWindow("\n\n"
        "*\n"
        "* Connection to the game has been dropped.\n"
        "*\n"
        "\n\n");
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        //QMessageBox::information(mainWindow, tr("Message"), tr("Unable to connect to server. Please check your internet connection and try again later."), QMessageBox::Ok, 0);

        windowManager->writeGameWindow("\n\n"
        "*\n"
        "* Unable to connect to server.\n"
        "*\n"
        "\n\n");

    } else if (error == QAbstractSocket::NetworkError) {
        //QMessageBox::information(mainWindow, tr("Message"), tr("Connection timed out."), QMessageBox::Ok, 0);

        windowManager->writeGameWindow("\n\n"
        "*\n"
        "* Connection timed out.\n"
        "*\n"
        "\n\n");
    }

    qDebug() << error;
}

ConnectionManager::~ConnectionManager() {
    tcpSocket->disconnect();

    delete tcpSocket;
}
