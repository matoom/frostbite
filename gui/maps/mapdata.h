#ifndef MAPDATA_H
#define MAPDATA_H

#include <QObject>

#include <maps/mapreader.h>
#include <maps/roomnode.h>

class MapReader;
class RoomNode;

class MapData : public QObject {
    Q_OBJECT

public:
    explicit MapData(MapReader* parent);

    QString findPath(QString zoneId, int startId, int destId);
    QString getZones();

    RoomNode findRoomNode(QString hash);

    void setRoom(RoomNode roomNode);
    RoomNode getRoom();

    RoomNode findLocation(QString keyword);

private:
    QReadWriteLock lock;

    MapReader* mapReader;
    RoomNode roomNode;

    QList<MapNode*> getShortestPathBFS(QHash<int, MapNode*>& nodes, int startId, int destId);
    QStringList getMoves(QList<MapNode*> path);

signals:

public slots:
};

#endif // MAPDATA_H
