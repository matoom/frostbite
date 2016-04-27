#include "mapwindow.h"

MapWindow::MapWindow(QWidget *parent) : QGraphicsView(parent) {
    mainWindow = (MainWindow*)parent;
    selected = NULL;

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setDragMode(QGraphicsView::ScrollHandDrag);

    this->scale(1.0, 1.0);
}

void MapWindow::setScene(QString zoneId, int level, QGraphicsScene* s) {
    if(this->zoneId != zoneId || this->level != level) {
        if(scene() != NULL && selected != NULL) {
           scene()->removeItem(selected);
           delete selected;
           selected = NULL;
        }
        QGraphicsView::setScene(s);

        this->zoneId = zoneId;
        this->level = level;
    }
}

void MapWindow::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 25) return;
    scale(scaleFactor, scaleFactor);
}

void MapWindow::wheelEvent(QWheelEvent* event) {
    scaleView(pow((double)2, event->delta() / 240.0));
}

void MapWindow::zoomIn() {
    scaleView(pow((double)2, 1 / 2.4));
}

void MapWindow::zoomOut() {
    scaleView(pow((double)2, -1 / 2.4));
}

void MapWindow::selectNode(MapZone* zone, int nodeId) {
    MapNode* node = zone->getNodes().value(nodeId);
    if(scene() != NULL) {
        if(selected == NULL) {
            selected = scene()->addEllipse(0, 0, 12, 12,  QColor("red"));
            selected->setPos(node->getPosition()->getX() + abs(zone->getXMin()) -4,
                             node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN -4);
        } else {
            selected->setPos(node->getPosition()->getX() + abs(zone->getXMin()) -4,
                             node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN -4);
        }
        viewport()->update();
    }
    centerOn(node->getPosition()->getX() + abs(zone->getXMin()), node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN);
}
