#ifndef MAPRECT_H
#define MAPRECT_H

#include <QObject>
#include <QDebug>
#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>

class MapRect : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    MapRect(qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b, QGraphicsItem *parent = 0);

    void setEndNode(QString endNode);
    QString getEndNode();

    void setNodeId(int nodeId);
    int getNodeId();

    void setZoneId(QString zoneId);
    QString getZoneId();

signals:
    void go(QString endNode, int level);
    void nodeSelected(QString zoneid, int nodeId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    QBrush brush;
    QRectF rect;

    QString endNode;

    QString zoneId;
    int nodeId;

};

#endif // MAPRECT_H
