#ifndef EAUTHSERVICE_H
#define EAUTHSERVICE_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QTcpSocket>

#include <clientsettings.h>
#include <connectionmanager.h>

class ClientSettings;
class ConnectionManager;

class EAuthService : public QObject {
    Q_OBJECT

public:
    explicit EAuthService(QObject *parent = 0);
    ~EAuthService();

    void init(QString, QString);
    void initiateSession();

private:
    ClientSettings* settings;
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    ConnectionManager *connectionManager;

    QString key;
    QString user;

    QString errorMessage;

    char* sge_encrypt_password(char *passwd, char *hash);
    void negotiateSession(QByteArray);

signals:
     void sessionKeyRecieved(QString);

public slots:

private slots:
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);
};

#endif // EAUTHSERVICE_H
