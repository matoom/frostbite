#ifndef MAPRECT_H
#define MAPRECT_H

#include <QObject>
#include <QWidget>

#include <QDebug>
#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsSceneMouseEvent>

class MapRect : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    MapRect(qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b, QGraphicsItem *parent = 0);
    ~MapRect();

    void setEndZone(QString endZoneId);
    QString getEndZone();

    void setNodeId(int nodeId);
    int getNodeId();

    void setZoneId(QString zoneId);
    QString getZoneId();

    void setLevel(int level);
    int getLevel();

    QGraphicsScene* getScene();
    void setScene(QGraphicsScene* scene);

signals:
    void go(QWidget*, QString endNode, int level);
    void nodeSelected(QWidget*, QString zoneid, int level, int nodeId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    QBrush brush;
    QRectF rect;

    QString endZoneId;

    QString zoneId;
    int nodeId;
    int level;

    int x;
    int y;
    int w;
    int h;

    QGraphicsScene* scene;
    QSharedPointer<QGraphicsTextItem> idTip;
};

#endif // MAPRECT_H
