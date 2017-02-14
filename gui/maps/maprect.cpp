#include "maprect.h"

MapRect::MapRect(qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, w, h, parent) {

    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setAcceptHoverEvents(true);
    this->setPen(p);
    this->setBrush(b);    
}

void MapRect::mousePressEvent(QGraphicsSceneMouseEvent* event) {       
    if(event->buttons() & Qt::LeftButton) {
        if(endZoneId != NULL) emit go(event->widget(), endZoneId, level);        
        emit nodeSelected(event->widget(), zoneId, level, nodeId);        
    }
    QGraphicsItem::mousePressEvent(event);
}

void MapRect::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {    
    setCursor(QCursor(Qt::PointingHandCursor));
    if (scene != NULL) {
        QGraphicsTextItem* tip = scene->addText("");
        idTip.reset(tip);
        idTip->setHtml("<div style=\"background:black;color:white;font-weight:bold;\">" +
                       QString::number(this->nodeId) +
                       "</div>");
        idTip->setPos(x + w + 2, y - 10);
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void MapRect::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(QCursor(Qt::PointingHandCursor));    
    if (scene != NULL) scene->removeItem(idTip.data());
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

QGraphicsScene* MapRect::getScene() {
    return this->scene;
}

void MapRect::setScene(QGraphicsScene* scene) {
    this->scene = scene;
}

MapRect::~MapRect() {
}
