#ifndef MAPDATA_H
#define MAPDATA_H

#include <QObject>
#include <QReadWriteLock>

#include "maps/roomnode.h"

class MapReader;
class MapNode;

class MapData : public QObject {
    Q_OBJECT

public:
    explicit MapData(MapReader* parent);

    QString findPath(QString zoneId, int startId, int destId);
    QString getZones();

    RoomNode findRoomNode(QString hash);

    void setRoom(const RoomNode& roomNode);
    const RoomNode& getRoom() const;

    RoomNode findLocation(QString keyword);

private:
    mutable QReadWriteLock lock;

    MapReader* mapReader;
    RoomNode roomNode;

    QList<MapNode*> getShortestPathBFS(QHash<int, MapNode*>& nodes, int startId, int destId);
    QStringList getMoves(QList<MapNode*> path);

signals:

public slots:
};

#endif // MAPDATA_H
