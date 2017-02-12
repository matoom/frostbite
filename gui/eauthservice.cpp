#include "eauthservice.h"

EAuthService::EAuthService(QObject *parent) : QObject(parent) {
    settings = new ClientSettings();
    tcpSocket = new QTcpSocket(this);
    tcpClient = (TcpClient*)parent;
    authLogger = new AuthLogger();

    if(tcpSocket) {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));

        connect(tcpSocket, SIGNAL(connected()),
                this, SLOT(startSession()));
    }

    connect(this, SIGNAL(selectGame(QMap<QString, QString>)),
            tcpClient, SLOT(selectGame(QMap<QString, QString>)));

    connect(this, SIGNAL(addCharacter(QString, QString)),
            tcpClient, SLOT(addCharacter(QString, QString)));

    connect(tcpClient, SIGNAL(retrieveSessionKey(QString)),
            this, SLOT(retrieveSessionKey(QString)));

    connect(tcpClient, SIGNAL(eAuthGameSelected(QString)),
            this, SLOT(gameSelected(QString)));

    connect(this, SIGNAL(sessionRetrieved(QString, QString, QString)),
            tcpClient, SLOT(eAuthSessionRetrieved(QString, QString, QString)));

    connect(this, SIGNAL(connectionError(QString)),
            tcpClient, SLOT(connectWizardError(QString)));

    connect(this, SIGNAL(authError()),
            tcpClient, SLOT(authError()));
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
    this->log("Negotiate session: ");
    this->write("K\n");
}

void EAuthService::retrieveSessionKey(QString id) {
    if(tcpSocket) {
        this->write("L\t" + id.toLocal8Bit() + "\tSTORM\n");
    }
}

void EAuthService::gameSelected(QString id) {
    this->gameId = id;

    if(tcpSocket) {
        this->write("F\t" + id.toLocal8Bit() + "\n");
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

QByteArray EAuthService::qt_sge_encrypt_password(QString passwd, QString hash) {
    QByteArray encrypted = "";
    for (int i = 0; i < passwd.length() && i < hash.length(); i++) {
        encrypted += QChar(((hash[i].toLatin1() ^ (passwd[i].toLatin1() - 32)) + 32));
    }
    encrypted += '\0';
    return encrypted;
}

void EAuthService::log(QByteArray buffer) {
    if(settings->getParameter("Logging/auth", false).toBool()) {
        authLogger->addText(buffer);
        if(!authLogger->isRunning()) {
            authLogger->start();
        }
    }
}

void EAuthService::write(QByteArray buffer) {
    this->log("> " + buffer);
    tcpSocket->write(buffer);
}

void EAuthService::negotiateSession(QByteArray buffer) {
    this->log(buffer);

    if(buffer.startsWith("A\t")) {
        QList<QByteArray> aResponse = buffer.split('\t');
        QByteArray aAction = aResponse.takeLast().trimmed();
        if(aAction == "REJECT") {
            emit connectionError("Subscription error.");
            tcpSocket->disconnectFromHost();
            return;
        } else if(aAction == "PASSWORD") {
            emit connectionError("Invalid user or password.");
            emit authError();
            tcpSocket->disconnectFromHost();
            return;
        }
        this->write("M\n");
    } else if(buffer.startsWith("M\t")) {
        QList<QByteArray> mResponse = buffer.trimmed().split('\t');
        mResponse.removeFirst();

        QMap<QString, QString> gameList;
        if(mResponse.length() % 2 == 0) {
            for(int i = 0; i < mResponse.length(); i = i + 2) {
                QString code = mResponse.at(i);
                QString name = mResponse.at(i + 1);
                if(name.toLower().contains("dragonrealms")) {
                    gameList.insert(name, code);
                }
            }
        } else {
            gameList.insert("Dragonrealms", "DR");
            gameList.insert("Dragonrealms The Fallen", "DRF");
            gameList.insert("Dragonrealms Prime Test", "DRT");
            gameList.insert("Dragonrealms Platinum", "DRX");
        }

        emit selectGame(gameList);
    } else if(buffer.startsWith("F\t")) {
        QList<QByteArray> fResponse = buffer.split('\t');
        if(fResponse.takeLast().trimmed() == "NEW_TO_GAME") {
            emit connectionError("Account error; character not found;");
            tcpSocket->disconnectFromHost();
            return;
        }
        this->write("G\t" + this->gameId.toLocal8Bit() + "\n");
    } else if(buffer.startsWith("G\t")) {
        this->write("P\t" + this->gameId.toLocal8Bit() + "\n");
    } else if(buffer.startsWith("P\t")) {
        this->write("C\n");
    } else if(buffer.startsWith("C\t")) {
        QRegExp rx("\\\t(\\w{2,})");
        int pos = 0;
        QStringList list;

        while ((pos = rx.indexIn(buffer, pos)) != -1) {
            list << rx.cap(1);
            pos += rx.matchedLength();
        }

        for(int i = 0; i < list.length(); i = i+2) {
            QString id = list[i];
            QString name = list[i+1];

            emit addCharacter(id, name);
        }
    } else if(buffer.startsWith("L\t")) {
        QList<QByteArray> lResponse = buffer.split('\t');

        emit sessionRetrieved(this->extractValue("GAMEHOST", lResponse),
                              this->extractValue("GAMEPORT", lResponse),
                              this->extractValue("KEY", lResponse));

        tcpSocket->disconnectFromHost();
    } else if(buffer.startsWith("X\t")) {
        emit connectionError("Unknown error; please refer to auth log.");
        emit authError();
        tcpSocket->disconnectFromHost();
    } else {
        if (buffer.size() == 33) {        
            QByteArray enc = qt_sge_encrypt_password(key, buffer);
            QString response = "A\t" + user.toUpper() + "\t" + enc + "\n";
            this->write(response.toLatin1());
        } else {
            this->log("Encountered unknown error!");
            emit connectionError("Unable to obtain session key. Try again.");
        }
    }
}

QByteArray EAuthService::extractValue(QByteArray key, QList<QByteArray> valueList) {
    foreach(QByteArray item, valueList) {
        if(item.startsWith(key)) {
            int index = item.trimmed().indexOf('=');
            return item.mid(index + 1).trimmed();
        }
    }
    return "";
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
    delete settings;
    delete tcpSocket;
}
