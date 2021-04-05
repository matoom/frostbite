#ifndef EAUTHSERVICE_H
#define EAUTHSERVICE_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QTcpSocket>

class ClientSettings;
class TcpClient;
class AuthLogger;

class EAuthService : public QObject {
    Q_OBJECT

public:
    explicit EAuthService(QObject *parent = 0);
    ~EAuthService();

    void init(QString, QString);
    void initSession(QString, QString);
    void resetSession();

private:
    ClientSettings* settings;
    QTcpSocket *tcpSocket;
    QByteArray buffer;
    TcpClient *tcpClient;
    AuthLogger* authLogger;

    QString key;
    QString user;

    QString gameId;

    QString errorMessage;

    char* sge_encrypt_password(char *passwd, char *hash);
    QByteArray qt_sge_encrypt_password(QString passwd, QString hash);

    void negotiateSession(QByteArray);    
    void write(QByteArray);
    void log(QByteArray );

    QByteArray extractValue(QByteArray, QList<QByteArray>);

signals:
     void sessionRetrieved(QString, QString, QString);
     void addCharacter(QString, QString);
     void selectGame(QMap<QString, QString>);
     void connectionError(QString);
     void authError();

public slots:
    void retrieveSessionKey(QString);
    void gameSelected(QString id);

private slots:
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);
    void startSession();
};

#endif // EAUTHSERVICE_H
