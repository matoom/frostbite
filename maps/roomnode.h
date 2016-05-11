#ifndef ROOMNODE_H
#define ROOMNODE_H

#include <QString>

class RoomNode {

public:
    RoomNode();
    RoomNode(QString zondeId, int level, int nodeId);

    QString getZoneId();
    int getLevel();
    int getNodeId();

private:
    QString zoneId;
    int level;
    int nodeId;
};

#endif // ROOMNODE_H
