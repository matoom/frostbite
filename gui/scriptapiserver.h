#ifndef SCRIPTAPISERVER_H
#define SCRIPTAPISERVER_H

#include <QObject>

#include <QtNetwork>

#include <gamedatacontainer.h>
#include <apisettings.h>

#include "log4qt/logger.h"

struct ApiRequest {
    QString name;
    QString arg;
};

class ScriptApiServer : public QObject {
    Q_OBJECT

public:
    explicit ScriptApiServer(QObject *parent = 0);

    QTcpServer *tcpServer;
    QNetworkSession *networkSession;

private:
    ApiRequest parseRequest(QString reqString);
    void write(QTcpSocket *socket, QString value);
    int boolToInt(bool value);

    GameDataContainer* data;
    ApiSettings* settings;

signals:

public slots:
        void newConnection();
        void sessionOpened();
        void readyRead();
};

#endif // SCRIPTAPISERVER_H
