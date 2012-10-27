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

    if(tcpSocket->state() == QAbstractSocket::HostLookupState) {
        tcpSocket->write("K\n");                
    }
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

void EAuthService::negotiateSession(QByteArray buffer) {
    qDebug() << "eauth-negotiate: " << buffer;
    if(buffer.startsWith("A\t")) {
        QList<QByteArray> answerList = buffer.split('\t');

        if(answerList.size() == 3) {
            //A..REJECT
            if(answerList.last().trimmed() == "REJECT") {
                connectionManager->showError("Account unavailable due to billing problems.");
                tcpSocket->disconnect();
            } else if(answerList.last().trimmed() == "PASSWORD") {
                connectionManager->showError("Incorrect password.");
            }
        } else if(answerList.size() == 5) {
            //A.CHARNAME.KEY.235cc51c4f94afa7cc49e376411f828e.JOE SCHMOE
            int index = answerList.indexOf("KEY");
            if(index != -1) {
                emit sessionKeyRecieved(answerList.at(index + 1));
            }
        }
    } else {
        if (buffer.size() == 33) {
            char* hash = sge_encrypt_password(key.toLocal8Bit().data(), buffer.data());

            QString response = "A\t" + user.toUpper() + "\t" + QString::fromLocal8Bit(hash) + "\n";
            tcpSocket->write(response.toLocal8Bit());
        } else {
            connectionManager->showError("Unable to obtain session key.");
        }
    }
}

void EAuthService::socketReadyRead() {
    this->negotiateSession(tcpSocket->readAll());
}

void EAuthService::socketError(QAbstractSocket::SocketError error) {
    if(error == QAbstractSocket::RemoteHostClosedError) {
        connectionManager->showError("Disconnected from server.");
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        connectionManager->showError("Unable to connect to server. Please check your internet connection and try again later.");
    } else if (error == QAbstractSocket::NetworkError) {
        connectionManager->showError("Connection timed out.");
    }

    /*QNativeSocketEngine::write() was not called in QAbstractSocket::ConnectedState
    QAbstractSocket::NetworkError*/
    //QAbstractSocket::RemoteHostClosedError

    qDebug() << "EAUTH: " <<  error;
}


EAuthService::~EAuthService() {
    tcpSocket->disconnect();
    delete tcpSocket;
}
