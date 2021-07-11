#include "scriptstreamserver.h"

#include "log4qt/logger.h"

#include "clientsettings.h"
#include "defaultvalues.h"

ScriptStreamServer::ScriptStreamServer(QObject* parent) : Parent(parent) {
    start();
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    connect(this, SIGNAL(writeText(QByteArray)), this, SLOT(sendMessage(QByteArray)));

    restart();
}

ScriptStreamServer::~ScriptStreamServer() {
    close();
}

void ScriptStreamServer::writeData(QString message) {
    // only add to the queue if we have sockets connected
    // and server is listening, no need to waste resources
    if (server.isListening() && sockets.size()) {
        Parent::addData(message);
    }
}

void ScriptStreamServer::onNewConnection() {
    QTcpSocket* clientSocket = server.nextPendingConnection();
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
            SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    sockets.push_back(clientSocket);
}

void ScriptStreamServer::onSocketStateChanged(QAbstractSocket::SocketState socketState) {
    if (socketState == QAbstractSocket::UnconnectedState) {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(sender);
    }
}

void ScriptStreamServer::reloadSettings() {
    restart();
}

void ScriptStreamServer::sendMessage(QByteArray message) {
    for (QTcpSocket* socket : sockets) {
        socket->write(message.append('\n'));
    }
}

void ScriptStreamServer::close() {
    for (QTcpSocket* socket : sockets) {
        socket->close();
    }
    sockets.clear();
    if (server.isListening()) {
        server.close();
    }
}

void ScriptStreamServer::restart() {
    bool enabled = ClientSettings::getInstance()
                           ->getParameter("Script/streamingServerEnabled", SCRIPT_STREAMING_ENABLED)
                           .toBool();
    int port = ClientSettings::getInstance()->getParameter("Script/streamingServerPort", 0).toInt();

    // check if need to close the server
    if (enabled && server.isListening() && port == server.serverPort()) {
        // no changes needed
        return;
    }

    close();
    // start server if necessary
    if (enabled) {
        if (!server.listen(QHostAddress::Any, port)) {
            Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))
                    ->info("Unable to start Streaming server" + server.errorString());
        }
    }
}
