#include "mapwindow.h"

#include <QGraphicsTextItem>

#include "globaldefines.h"
#include "custom/contextmenu.h"
#include "maps/mapzone.h"
#include "maps/mapfacade.h"
#include "maps/mapreader.h"
#include "maps/mapnode.h"
#include "generalsettings.h"
#include "textutils.h"
#include "gamedatacontainer.h"

MapWindow::MapWindow(MapFacade *parent) : QGraphicsView() {
    mapFacade = parent;
    selected = NULL;

    settings = GeneralSettings::getInstance();
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

    QString zoomLabel = QString("%1x").arg(scale * scaleFactor, 0, 'f', 2);
    mapFacade->setZoom(zoomLabel);
}

void MapWindow::wheelEvent(QWheelEvent* event) {
    int delta = event->delta();
    if(delta != 0) {
        scaleView(delta / qFabs(delta * 4));
    }
}

void MapWindow::zoomIn() {
    scaleView(0.25);
}

void MapWindow::zoomOut() {
    scaleView(-0.25);
}

void MapWindow::reset() {
    emit updateMapWindow(GameDataContainer::Instance()->getNodeHash());
}

void MapWindow::selectNode(MapZone* zone, int nodeId) {    
    MapNode* node = zone->getNodes().value(nodeId);
    centerOn(node->getPosition().getX() + abs(zone->getXMin()),
             node->getPosition().getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN);
}

void MapWindow::buildContextMenu() {
    menu = new ContextMenu(this);
    QAction* mapsAction = new QAction(tr("&View maps\t"), this);
    menu->addAction(mapsAction);

    connect(mapsAction, SIGNAL(triggered()), mapFacade, SLOT(showMapDialog()));
}

void MapWindow::contextMenuEvent(QContextMenuEvent* event) {
    QPoint point = event->globalPos();
    point.rx()--; point.ry()--;
    menu->exec(point);
}

MapWindow::~MapWindow() {
}
