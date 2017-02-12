#include "mapwindow.h"

MapWindow::MapWindow(MapFacade *parent) : QGraphicsView() {
    mapFacade = parent;
    selected = NULL;

    settings = new GeneralSettings();
    this->setBackgroundBrush(QBrush(settings->dockWindowBackground()));

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setDragMode(QGraphicsView::ScrollHandDrag);

    this->scale(1.0, 1.0);

    this->buildContextMenu();
    this->setLoadingMessage();
}

void MapWindow::setLoadingMessage() {
    QGraphicsScene* scene = new QGraphicsScene(0, 0, 0, 0, this);
    QGraphicsTextItem* text = scene->addText("Loading ..");
    text->setDefaultTextColor(mapFacade->getMapReader()->getTextColor(settings->dockWindowBackground()));
    this->setScene(scene);
}

void MapWindow::scaleView(qreal step) {
    qreal scale = transform().scale(1, 1).mapRect(QRectF(0, 0, 1, 1)).width();

    if ((step < 0 && scale <= 0.5) || (step > 0 && scale >= 5)) return;

    qreal scaleFactor = (scale + step) / (qreal)scale;

    this->scale(scaleFactor, scaleFactor);
    mapFacade->setZoom(QString::number(scale * scaleFactor) + "x");
}

void MapWindow::wheelEvent(QWheelEvent* event) {
    scaleView(event->delta() / qFabs(event->delta() * 2));
}

void MapWindow::zoomIn() {
    scaleView(0.5);
}

void MapWindow::zoomOut() {
    scaleView(-0.5);
}

void MapWindow::reset() {
    QString text = GameDataContainer::Instance()->getRoomName() +
            GameDataContainer::Instance()->getRoomDesc() +
            GameDataContainer::Instance()->getCompassDirections().join("");

    emit updateMapWindow(TextUtils::toHash(text));
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
