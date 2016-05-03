#include "maprect.h"

MapRect::MapRect(qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, w, h, parent) {

    this->setAcceptHoverEvents(true);
    this->setPen(p);
    this->setBrush(b);
}

void MapRect::mousePressEvent(QGraphicsSceneMouseEvent* event) {   
    QGraphicsItem::mousePressEvent(event);
    if(event->buttons() & Qt::LeftButton) {
        if(endZoneId != NULL) emit go(event->widget(), endZoneId, level);
        emit nodeSelected(event->widget(), zoneId, level, nodeId);
    }
}

void MapRect::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(QCursor(Qt::PointingHandCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void MapRect::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(QCursor(Qt::PointingHandCursor));
    QGraphicsItem::hoverLeaveEvent(event);
}

void MapRect::setEndZone(QString endZoneId) {
    this->endZoneId = endZoneId;
}

QString MapRect::getEndZone() {
    return this->endZoneId;
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

void MapRect::setLevel(int level) {
    this->level = level;
}

int MapRect::getLevel() {
    return this->level;
}
