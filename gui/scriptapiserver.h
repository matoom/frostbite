#ifndef SCRIPTAPISERVER_H
#define SCRIPTAPISERVER_H

#include <QObject>
#include <QStringList>

#include <QtNetwork>

#include <gamedatacontainer.h>
#include <apisettings.h>
#include <mainwindow.h>
#include <maps/mapdata.h>
#include <tcpclient.h>

#include "log4qt/logger.h"

struct ApiRequest {
    QString name;
    QStringList args;
};

class MapData;
class TcpClient;
class GridWindow;

class ScriptApiServer : public QObject {
    Q_OBJECT

public:
    explicit ScriptApiServer(QObject *parent = 0);
    ~ScriptApiServer();

    QTcpServer *tcpServer;
    QNetworkSession *networkSession;

private:
    ApiRequest parseRequest(QString reqString);
    void write(QTcpSocket *socket, QString value);
    int boolToInt(bool value);

    GameDataContainer* data;
    ApiSettings* settings;

    MainWindow* mainWindow;
    MapData* mapData;

    TcpClient* tcpClient;
    GridWindow* expWindow;

signals:
    void track(QString);
    void clearTracked();

public slots:
        void newConnection();
        void sessionOpened();
        void readyRead();
};

#endif // SCRIPTAPISERVER_H
