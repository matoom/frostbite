#include "roomnode.h"

RoomNode::RoomNode() {
    this->zoneId = "";
    this->level = -1;
    this->nodeId = -1;
}

RoomNode::RoomNode(QString zoneId, int level, int nodeId) {
    this->zoneId = zoneId;
    this->level = level;
    this->nodeId = nodeId;
}

QString RoomNode::getZoneId() {
    return this->zoneId;
}

int RoomNode::getLevel() {
    return this->level;
}

int RoomNode::getNodeId() {
    return this->nodeId;
}
