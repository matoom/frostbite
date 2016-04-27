#include "maprect.h"

MapRect::MapRect(qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, w, h, parent) {

    this->setAcceptHoverEvents(true);
    this->setPen(p);
    this->setBrush(b);
}

void MapRect::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
    if(endNode != NULL) emit go(endNode, 0);
    emit nodeSelected(zoneId, nodeId);
}

void MapRect::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(QCursor(Qt::PointingHandCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void MapRect::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(QCursor(Qt::PointingHandCursor));
    QGraphicsItem::hoverLeaveEvent(event);
}

void MapRect::setEndNode(QString endNode) {
    this->endNode = endNode;
}

QString MapRect::getEndNode() {
    return this->endNode;
}

void MapRect::setNodeId(int nodeId) {
    this->nodeId = nodeId;
}

int MapRect::getNodeId() {
    return this->nodeId;
}

void MapRect::setZoneId(QString zoneId) {
    this->zoneId = zoneId;
}

QString MapRect::getZoneId() {
    return this->zoneId;
}
