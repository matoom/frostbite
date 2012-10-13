#include "eauthservice.h"

EAuthService::EAuthService(QObject *parent) : QObject(parent) {
    settings = ClientSettings::Instance();
    tcpSocket = new QTcpSocket(this);
    connectionManager = (ConnectionManager*)parent;

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(this, SIGNAL(sessionKeyRecieved(QString)), connectionManager, SLOT(connectToHost(QString)));
    }
}

void EAuthService::init(QString user, QString key) {
    this->user = user;
    this->key = key;
}

void EAuthService::initiateSession() {
    QString host = settings->getParameter("Login/authHost", "").toString();
    int port = settings->getParameter("Login/authPort", "").toInt();

    tcpSocket->connectToHost(host, port);
    tcpSocket->write("K\n");
}

/* @source: http://warlockclient.wikia.com/wiki/EAccess_Protocol */
char* EAuthService::sge_encrypt_password(char *passwd, char *hash) {
    char *final = (char*)malloc(sizeof (char)* 33);

    int i;
    for (i = 0; i < 32 && passwd[i] != '\0' && hash[i] != '\0'; i++) {
        final[i] = (char)((hash[i] ^ (passwd[i] - 32)) + 32);
    }
    final[i] = '\0';

    return final;
}

void EAuthService::nogotiateSession(QByteArray buffer) {
    if(buffer.startsWith("A\t")) {
        QList<QByteArray> keyList = buffer.split('\t');

        int index = keyList.indexOf("KEY");
        if(index != -1) {
            sessionKeyRecieved(keyList.at(index + 1));
        }

    } else {
        if (buffer.size() == 33) {
            char* hash = sge_encrypt_password(key.toLocal8Bit().data(), buffer.data());

            QString response = "A\t" + user.toUpper() + "\t" + QString::fromLocal8Bit(hash) + "\n";
            tcpSocket->write(response.toLocal8Bit());
        } else {
            errorMessage = "Unable to get session key.";
        }
    }
}

void EAuthService::socketReadyRead() {
    this->nogotiateSession(tcpSocket->readAll());
}

void EAuthService::socketError(QAbstractSocket::SocketError error) {
    if(error == QAbstractSocket::RemoteHostClosedError) {
        errorMessage = "Disconnected from server.";
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        errorMessage = "Unable to connect to server. Please check your internet connection and try again later.";
    } else if (error == QAbstractSocket::NetworkError) {
        errorMessage = "Connection timed out.";
    }


    /*QNativeSocketEngine::write() was not called in QAbstractSocket::ConnectedState
    QAbstractSocket::NetworkError*/
    //QAbstractSocket::RemoteHostClosedError

    qDebug() << error;
}


EAuthService::~EAuthService() {
    tcpSocket->disconnect();
    delete tcpSocket;
}
