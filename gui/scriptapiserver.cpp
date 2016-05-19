#include "scriptapiserver.h"

ScriptApiServer::ScriptApiServer(QObject *parent) : QObject(parent), networkSession(0) {
    mainWindow = (MainWindow*)parent;
    mapData = mainWindow->getWindowFacade()->getMapFacade()->getData();
    tcpClient = mainWindow->getTcpClient();

    data = GameDataContainer::Instance();    

    settings = new ApiSettings();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        networkSession = new QNetworkSession(manager.defaultConfiguration(), this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));
        networkSession->open();
    } else {
        sessionOpened();
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void ScriptApiServer::sessionOpened() {
    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress::LocalHost)) {
        Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->
                info("Unable to start server" + tcpServer->errorString());
        return;
    }
    settings->setParameter("ApiServer/port", tcpServer->serverPort());
}

void ScriptApiServer::newConnection() {
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    clientConnection->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void ScriptApiServer::readyRead() {
    QTcpSocket *socket = (QTcpSocket*) sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        if(line.startsWith("GET")) {
            ApiRequest request = parseRequest(line.mid(3).trimmed());
            if(request.name == "EXP_RANK") {
                this->write(socket, tr("%1\\0").arg(data->getExp(request.args.at(0)).value("rank")));
            } else if(request.name == "EXP_STATE") {
                this->write(socket, tr("%1\\0").arg(data->getExp(request.args.at(0)).value("state")));
            } else if(request.name == "ACTIVE_SPELLS") {
                this->write(socket, tr("%1\\0").arg(data->getActiveSpells().join("\n")));
            } else if(request.name == "INVENTORY") {
                this->write(socket, tr("%1\\0").arg(data->getInventory().join("\n")));
            } else if(request.name == "CONTAINER") {
                this->write(socket, tr("%1\\0").arg(data->getContainer().join("\n")));
            } else if(request.name == "WIELD_RIGHT") {
                this->write(socket, tr("%1\\0").arg(data->getRight()));
            } else if(request.name == "WIELD_RIGHT_NOUN") {
                this->write(socket, tr("%1\\0").arg(data->getRightNoun()));
            } else if(request.name == "WIELD_LEFT") {
                this->write(socket, tr("%1\\0").arg(data->getLeft()));
            } else if(request.name == "WIELD_LEFT_NOUN") {
                this->write(socket, tr("%1\\0").arg(data->getLeftNoun()));
            } else if(request.name == "HEALTH") {
                this->write(socket, tr("%1\\0").arg(data->getHealth()));
            } else if(request.name == "CONCENTRATION") {
                this->write(socket, tr("%1\\0").arg(data->getConcentration()));
            } else if(request.name == "SPIRIT") {
                this->write(socket, tr("%1\\0").arg(data->getSpirit()));
            } else if(request.name == "FATIGUE") {
                this->write(socket, tr("%1\\0").arg(data->getFatigue()));
            } else if(request.name == "STANDING") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getStanding())));
            } else if(request.name == "SITTING") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getSitting())));
            } else if(request.name == "KNEELING") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getKneeling())));
            } else if(request.name == "PRONE") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getProne())));
            } else if(request.name == "STUNNED") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getStunned())));
            } else if(request.name == "BLEEDING") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getBleeding())));
            } else if(request.name == "HIDDEN") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getHidden())));
            } else if(request.name == "INVISIBLE") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getInvisible())));
            } else if(request.name == "WEBBED") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getWebbed())));
            } else if(request.name == "JOINED") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getJoined())));
            } else if(request.name == "DEAD") {
                this->write(socket, tr("%1\\0").arg(boolToInt(data->getDead())));
            } else if(request.name == "ROOM_TITLE") {
                this->write(socket, tr("%1\\0").arg(data->getRoomName()));
            } else if(request.name == "ROOM_DESC") {
                this->write(socket, tr("%1\\0").arg(data->getRoomDesc()));
            } else if(request.name == "ROOM_OBJECTS") {
                this->write(socket, tr("%1\\0").arg(data->getRoomObjs()));
            } else if(request.name == "ROOM_PLAYERS") {
                this->write(socket, tr("%1\\0").arg(data->getRoomPlayers()));
            } else if(request.name == "ROOM_EXITS") {
                this->write(socket, tr("%1\\0").arg(data->getRoomExits()));
            } else if(request.name == "RT") {
                this->write(socket, tr("%1\\0").arg(data->getRt()));
            } else {
                this->write(socket, tr("\\0"));
            }
        } else if(line.startsWith("MAP_GET")) {
            ApiRequest request = parseRequest(line.mid(7).trimmed());
            if(request.name == "PATH") {
                QStringList args = request.args;
                if(args.size() < 3) {
                    this->write(socket, tr("\\0"));
                } else {
                    QString path = mapData->findPath(args.at(0), args.at(1).toInt(), args.at(2).toInt());
                    this->write(socket, tr("%1\\0").arg(path));
                }
            } else if(request.name == "CURRENT_ROOM") {
                RoomNode room = mapData->getRoom();
                this->write(socket, QString("{:zone => '%1', :level => %2, :id => %3}\\0")
                            .arg(room.getZoneId(), QString::number(room.getLevel()), QString::number(room.getNodeId())));
            } else if(request.name == "ZONES") {
                this->write(socket, tr("%1\\0").arg(mapData->getZones()));                
            } else if(request.name == "FIND_ROOM") {
                QStringList args = request.args;
                if(args.size() < 1) {
                    this->write(socket, tr("\\0"));
                } else {
                    RoomNode room = mapData->findLocation(args.at(0));
                    this->write(socket, QString("{:zone => '%1', :level => %2, :id => %3}\\0")
                                .arg(room.getZoneId(), QString::number(room.getLevel()), QString::number(room.getNodeId())));
                }
            } else {
                this->write(socket, tr("\\0"));
            }
        } else if(line.startsWith("CLIENT")) {
            ApiRequest request = parseRequest(line.mid(6).trimmed());
            if(request.name == "CONNECT") {
                QStringList args = request.args;
                if(args.size() < 6) {
                    this->write(socket, tr("1\\0"));
                } else {
                    tcpClient->connectApi(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5));
                    this->write(socket, tr("0\\0"));
                }
            }
        } else {
            this->write(socket, tr("\\0"));
        }
    }
}

void ScriptApiServer::write(QTcpSocket *socket, QString value) {
    socket->write(value.toLocal8Bit());
    socket->flush();
}

ApiRequest ScriptApiServer::parseRequest(QString reqString) {
    ApiRequest apiRequest;

    int index = reqString.indexOf("?");
    if(index > -1) {
        apiRequest.name = reqString.mid(0, index);
        apiRequest.args = reqString.mid(index + 1).split("&");
    } else {
        apiRequest.name = reqString;
        apiRequest.args = QStringList();
    }
    return apiRequest;
}

int ScriptApiServer::boolToInt(bool value) {
    if(value) {
        return 1;
    } else {
        return 0;
    }
}




