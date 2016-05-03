#include "mapwindow.h"

MapWindow::MapWindow(MapFacade *parent) : QGraphicsView() {
    mapFacade = parent;
    selected = NULL;

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setDragMode(QGraphicsView::ScrollHandDrag);

    this->scale(1.0, 1.0);

    this->buildContextMenu();
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
    centerOn(node->getPosition()->getX() + abs(zone->getXMin()), node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN);
}

void MapWindow::buildContextMenu() {
    menu = new QMenu(this);

    QAction* mapsAction = new QAction(tr("&View maps\t"), this);
    menu->addAction(mapsAction);

    connect(mapsAction, SIGNAL(triggered()), mapFacade, SLOT(showMapDialog()));
}

void MapWindow::contextMenuEvent(QContextMenuEvent* event) {
    menu->exec(event->globalPos());
}
