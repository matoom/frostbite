#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QDebug>
#include <QWheelEvent>

#include <maps/mapzone.h>
#include <maps/mapfacade.h>

class MapFacade;

class MapWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit MapWindow(MapFacade* parent);

private:
    void wheelEvent(QWheelEvent* zoomEvent);
    void contextMenuEvent(QContextMenuEvent* event);

    void scaleView(qreal);
    void buildContextMenu();

    MapFacade* mapFacade;
    QGraphicsEllipseItem* selected;

    QString zoneId;
    int level;

    QMenu* menu;

signals:
    void updateMapWindow(QString);

public slots:
    void zoomIn();
    void zoomOut();
    void selectNode(MapZone* zone, int nodeId);
    void reset();
};

#endif // MAPWINDOW_H
