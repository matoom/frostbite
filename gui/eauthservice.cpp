#include "eauthservice.h"

#include "clientsettings.h"
#include "authlogger.h"
#include "tcpclient.h"

EAuthService::EAuthService(QObject *parent) : QObject(parent) {
    settings = ClientSettings::getInstance();   

    tcpClient = (TcpClient*)parent;
    authLogger = new AuthLogger();

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

    connect(this, SIGNAL(connectionWarning(QString)),
            tcpClient, SLOT(connectionWarning(QString)));

    connect(this, SIGNAL(authError()),
            tcpClient, SLOT(authError()));

    sslSocket = new QSslSocket(this);
    sslSocket->setProtocol(QSsl::AnyProtocol);

    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(startSession()));

    loadSslCertificate();
}

void EAuthService::init(QString user, QString key) {
    this->user = user;
    this->key = key;
}

void EAuthService::resetSession() {
    sslSocket->disconnectFromHost();
}

void EAuthService::initSession(QString host, QString port) {
    if(sslSocket->state() == QAbstractSocket::UnconnectedState) {
        sslSocket->connectToHostEncrypted(host, port.toInt());
    }
}

void EAuthService::loadSslCertificate() {
    const QString serverCertPath(QApplication::applicationDirPath() + "/security/auth-service.pem");
    QList<QSslCertificate> serverCert = QSslCertificate::fromPath(serverCertPath, QSsl::Pem);
    if(serverCert.isEmpty()) {
        emit connectionWarning("WARNING: Unable to load auth service SSL certificate from " + serverCertPath + "; SSL verification turned off.");
        sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
        connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(logAndIgnoreSslErrors(QList<QSslError>)));
    } else {
        sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);
        QList<QSslError> verifiedErrors;
        verifiedErrors << QSslError(QSslError::HostNameMismatch, serverCert.at(0));
        verifiedErrors << QSslError(QSslError::SelfSignedCertificate, serverCert.at(0));
        verifiedErrors << QSslError(QSslError::CertificateUntrusted, serverCert.at(0));
        sslSocket->ignoreSslErrors(verifiedErrors);
    }
}

void EAuthService::logAndIgnoreSslErrors(const QList<QSslError>& errors) {
    for(QSslError error : errors) {
        this->log(error.errorString().toLocal8Bit());
        emit connectionWarning("WARNING: " + error.errorString());
    }
    sslSocket->ignoreSslErrors(errors);
}

void EAuthService::startSession() {
    this->log("Negotiate session: ");
    this->write("K");
}

void EAuthService::retrieveSessionKey(QString id) {
    if(sslSocket) {
        this->write("L\t" + id.toLocal8Bit() + "\tSTORM");
    }
}

void EAuthService::gameSelected(QString id) {
    this->gameId = id;

    if(sslSocket) {
        this->write("F\t" + id.toLocal8Bit());
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
    sslSocket->write(buffer);
}

void EAuthService::negotiateSession(QByteArray buffer) {
    this->log(buffer);

    if(buffer.startsWith("A\t")) {
        QList<QByteArray> aResponse = buffer.split('\t');
        QByteArray aAction = aResponse.takeLast().trimmed();
        if(aAction == "REJECT") {
            emit connectionError("Subscription error.");
            this->socketDisconnect();
            return;
        } else if(aAction == "PASSWORD") {
            emit connectionError("Invalid user or password.");
            emit authError();
            this->socketDisconnect();
            return;
        }
        this->write("M");
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
        QByteArray msg = fResponse.takeLast().trimmed();
        if(msg == "NEW_TO_GAME" || msg == "NEED_BILL") {
            emit connectionError("Account error; character not found;");
            this->socketDisconnect();
            return;
        }
        this->write("G\t" + this->gameId.toLocal8Bit());
    } else if(buffer.startsWith("G\t")) {
        this->write("P\t" + this->gameId.toLocal8Bit());
    } else if(buffer.startsWith("P\t")) {
        this->write("C");
    } else if(buffer.startsWith("C\t")) {
        QList<QByteArray> list = buffer.split('\t');
        QList<QByteArray> accounts = list.mid(5);

        for(int i = 0; i < accounts.length(); i = i + 2) {
            QString id = accounts[i];
            QString name = accounts[i + 1];

            emit addCharacter(id, name.trimmed());
        }
    } else if(buffer.startsWith("L\t")) {
        QList<QByteArray> lResponse = buffer.split('\t');

        QString gamehost = this->extractValue("GAMEHOST", lResponse);
        QString gamePort = this->extractValue("GAMEPORT", lResponse);
        QString key = this->extractValue("KEY", lResponse);

        this->log(QString("Session retrieved for " + gamehost + ":"
                          + gamePort + " with key \"" + key + "\"").toLocal8Bit());

        emit sessionRetrieved(gamehost, gamePort, key);
        this->socketDisconnect();
    } else if(buffer.startsWith("X\t")) {
        emit connectionError("Unknown error; please refer to auth log.");
        emit authError();
        this->socketDisconnect();
    } else {
        if (buffer.size() == 32) {
            QByteArray enc = qt_sge_encrypt_password(key, buffer);
            QString response = "A\t" + user.toUpper() + "\t" + enc;
            this->write(response.toLatin1());
        } else {
            this->log("Encountered unknown error!");
            emit connectionError("Unable to obtain session key. Try again.");
            this->socketDisconnect();
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
    this->negotiateSession(sslSocket->readAll());
}

void EAuthService::socketDisconnect() {
    if(sslSocket->state() == QAbstractSocket::ConnectedState) {
        sslSocket->disconnectFromHost();
        if(sslSocket->state() != QAbstractSocket::UnconnectedState) {
            sslSocket->waitForDisconnected();
        }
    }
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
        emit connectionError("Network error: QAbstractSocket::" + QVariant::fromValue(error).toString());
    }
    qDebug() << "EAUTH: " <<  error;
}

EAuthService::~EAuthService() {
    this->socketDisconnect();
    delete sslSocket;
    delete authLogger;
}
