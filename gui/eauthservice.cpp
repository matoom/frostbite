#include "eauthservice.h"

EAuthService::EAuthService(QObject *parent) : QObject(parent) {
    settings = ClientSettings::Instance();
    tcpSocket = new QTcpSocket(this);
    connectionManager = (ConnectionManager*)parent;

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));

        connect(tcpSocket, SIGNAL(connected()),
                this, SLOT(startSession()));
    }

    connect(this, SIGNAL(selectGame()),
            connectionManager, SLOT(selectGame()));

    connect(this, SIGNAL(addCharacter(QString, QString)),
            connectionManager, SLOT(addCharacter(QString, QString)));

    connect(connectionManager, SIGNAL(retrieveSessionKey(QString)),
            this, SLOT(retrieveSessionKey(QString)));

    connect(connectionManager, SIGNAL(eAuthGameSelected(QString)),
            this, SLOT(gameSelected(QString)));

    connect(this, SIGNAL(sessionRetrieved(QString, QString, QString)),
            connectionManager, SLOT(eAuthSessionRetrieved(QString, QString, QString)));

    connect(this, SIGNAL(connectionError(QString)),
            connectionManager, SLOT(connectWizardError(QString)));

    connect(this, SIGNAL(authError()),
            connectionManager, SLOT(authError()));
}

void EAuthService::init(QString user, QString key) {
    this->user = user;
    this->key = key;
}

void EAuthService::resetSession() {
    tcpSocket->disconnectFromHost();
}

void EAuthService::initSession(QString host, QString port) {
    if(tcpSocket->state() == QAbstractSocket::UnconnectedState) {
        tcpSocket->connectToHost(host, port.toInt());
    }
}

void EAuthService::startSession() {
    tcpSocket->write("K\n");
}

void EAuthService::retrieveSessionKey(QString id) {
    if(tcpSocket) {
        tcpSocket->write("L\t" + id.toLocal8Bit() + "\tSTORM\n");
    }
}

void EAuthService::gameSelected(QString id) {
    this->gameId = id;

    if(tcpSocket) {
        tcpSocket->write("F\t" + id.toLocal8Bit() + "\n");
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
    if(buffer.startsWith("A\t")) {
        QList<QByteArray> aResponse = buffer.split('\t');
        if(aResponse.takeLast().trimmed() == "REJECT") {
            emit connectionError("Subscription error.");
            tcpSocket->disconnectFromHost();
            return;
        }
        tcpSocket->write("M\n");
    } else if(buffer.startsWith("M\t")) {
        emit selectGame();
    } else if(buffer.startsWith("F\t")) {
        QList<QByteArray> fResponse = buffer.split('\t');
        if(fResponse.takeLast().trimmed() == "NEW_TO_GAME") {
            emit connectionError("Subscription error.");
            tcpSocket->disconnectFromHost();
            return;
        }
        tcpSocket->write("G\t" + this->gameId.toLocal8Bit() + "\n");
    } else if(buffer.startsWith("G\t")) {
        tcpSocket->write("P\t" + this->gameId.toLocal8Bit() + "\n");
    } else if(buffer.startsWith("P\t")) {
        tcpSocket->write("C\n");
    } else if(buffer.startsWith("C\t")) {
        QList<QByteArray> cResponse = buffer.split('\t');
        emit addCharacter(QString::fromLocal8Bit(cResponse.takeLast()).trimmed(),
                QString::fromLocal8Bit(cResponse.takeLast().trimmed()));
    } else if(buffer.startsWith("L\t")) {
        QList<QByteArray> lResponse = buffer.split('\t');

        emit sessionRetrieved(this->extractValue(lResponse.takeLast()),
                              this->extractValue(lResponse.takeLast()),
                              this->extractValue(lResponse.takeLast().trimmed()));

        tcpSocket->disconnectFromHost();
    } else if(buffer.startsWith("X\t")) {
        emit connectionError("Invalid user or password.");
        emit authError();
        tcpSocket->disconnectFromHost();
    } else {
        if (buffer.size() == 33) {
            char* hash = sge_encrypt_password(key.toLocal8Bit().data(), buffer.data());

            QString response = "A\t" + user.toUpper() + "\t" + QString::fromLocal8Bit(hash) + "\n";
            tcpSocket->write(response.toLocal8Bit());
        } else {
            emit connectionError("Unable to obtain session key.");
        }
    }
}

QByteArray EAuthService::extractValue(QByteArray valuePair) {
    int index = valuePair.indexOf('=');
    return valuePair.mid(index + 1);
}

void EAuthService::socketReadyRead() {
    this->negotiateSession(tcpSocket->readAll());
}

void EAuthService::socketError(QAbstractSocket::SocketError error) {
    if(error == QAbstractSocket::RemoteHostClosedError) {
        emit connectionError("Disconnected from auth server.");
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        emit connectionError("Unable to connect to auth server.");
    } else if (error == QAbstractSocket::NetworkError) {
        emit connectionError("Connection timed out.");
    } else if (error == QAbstractSocket::HostNotFoundError) {
        emit connectionError("Unable to connect to auth server. Host not found.");
    } else {
        emit connectionError("Network error.");
    }

    qDebug() << "EAUTH: " <<  error;
}

EAuthService::~EAuthService() {
    tcpSocket->disconnectFromHost();
    delete tcpSocket;
}
