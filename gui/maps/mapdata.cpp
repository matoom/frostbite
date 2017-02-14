#include "mapdata.h"

MapData::MapData(MapReader* parent) : QObject(parent) {
    mapReader = parent;
}

QString MapData::getZones() {
    return mapReader->getZones().keys().join(",");
}

void MapData::setRoom(RoomNode roomNode) {
    QWriteLocker locker(&lock);
    this->roomNode = roomNode;
}

RoomNode MapData::getRoom() {
    QReadLocker locker(&lock);
    return this->roomNode;
}

RoomNode MapData::findLocation(QString note) {
    QHash<QString, RoomNode> locations = mapReader->getLocations();
    QHash<QString, RoomNode>::iterator i = locations.find(note);

    if(i == locations.end()) {
        return RoomNode();
    } else {
        return i.value();
    }
}

QString MapData::findPath(QString zoneId, int startId, int destId) {
    MapZone* zone = mapReader->getZones().value(zoneId);

    if(zone == NULL) return "";

    QHash<int, MapNode*>& nodes = zone->getNodes();

    if(!nodes.contains(startId) && !nodes.contains(destId)) return "";

    QList<MapNode*> path = this->getShortestPathBFS(nodes, startId, destId);

    return this->getMoves(path).join(",");
}

QList<MapNode*> MapData::getShortestPathBFS(QHash<int, MapNode*>& nodes, int startId, int finishId) {
    QList<int> ids = nodes.keys();

    QHash<int, bool> visited;
    foreach(int id, ids) visited.insert(id, false);

    QQueue<MapNode*> queue;

    visited.insert(startId, true);

    queue.push_back(nodes.value(startId));

    QHash<MapNode*, MapNode*> prev;

    MapNode* currentNode = NULL;
    while(!queue.isEmpty()) {
        currentNode = queue.front();

        if(currentNode->getId() == finishId) break;

        queue.pop_front();        

        QMultiHash<int, MapDestination* >& destinations = currentNode->getDestinations();

        QList<MapDestination*> values = destinations.values();
        for (int i = 0; i < values.size(); ++i) {
            int destId = values.at(i)->getDestId();
            if(!visited.value(destId) && destId != -1) {
                visited.insert(destId, true);

                MapNode* destNode = nodes.value(destId);                                                
                if(destNode != NULL) {
                    queue.push_back(destNode);
                    prev.insert(destNode, currentNode);
                }
            }
        }
    }

    MapNode* finish = nodes.value(finishId);

    if(finish == NULL || currentNode != finish) return QList<MapNode*>();

    QList<MapNode*> path;
    for(MapNode* node = finish; node != NULL; node = prev.value(node)) {
        path.push_front(node);
    }

    return path;
}

QStringList MapData::getMoves(QList<MapNode*> path) {
    QStringList moves;

    MapNode* prev = path.first();
    for(int i = 1; i < path.size(); ++i) {
        MapNode* current = path.value(i);
        QString move = prev->getDestinations().value(current->getId())->getMove();
        if(move.isEmpty()) {
            moves << prev->getDestinations().value(current->getId())->getExit();
        } else {
            moves << move;
        }
        prev = current;
    }
    return moves;
}

RoomNode MapData::findRoomNode(QString hash) {
    QList<RoomNode> nodes = mapReader->getRoomNodes().values(hash);
    if(nodes.isEmpty()) return RoomNode();
    return nodes.last();
}
