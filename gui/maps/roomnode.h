#ifndef ROOMNODE_H
#define ROOMNODE_H

#include <QString>

class RoomNode {

public:
    RoomNode();
    RoomNode(QString zondeId, int level, int nodeId);

    const QString& getZoneId() const;
    int getLevel() const;
    int getNodeId() const;

private:
    QString zoneId;
    int level;
    int nodeId;
};

#endif // ROOMNODE_H
