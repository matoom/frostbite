#include "mapdialog.h"
#include "ui_mapdialog.h"

#include "maps/mapzone.h"
#include "maps/mapfacade.h"
#include "maps/mapreader.h"
#include "maps/mapdata.h"
#include "maps/roomnode.h"
#include "maps/mapdestination.h"
#include "maps/mapnode.h"
#include "generalsettings.h"
#include "globaldefines.h"

MapDialog::MapDialog(MapFacade* mapFacade, QWidget *parent) : QDialog(parent), ui(new Ui::MapDialog) {
    ui->setupUi(this);

    this->mapFacade = mapFacade;

    settings = GeneralSettings::getInstance();

    ui->mapView->setScene(new QGraphicsScene(0, 0, 0, 0, this));
    this->setBackgroundBrush(QBrush(settings->dockWindowBackground()));

    connect(ui->mapSelect, SIGNAL(activated(int)), this, SLOT(mapSelected(int)));
    connect(ui->levelSelect, SIGNAL(activated(int)), this, SLOT(mapLevelSelected(int)));
    connect(ui->zoomIn, SIGNAL(pressed()), this, SLOT(zoomIn()));
    connect(ui->zoomOut, SIGNAL(pressed()), this, SLOT(zoomOut()));
    connect(ui->reset, SIGNAL(pressed()), this, SLOT(reset()));

    ui->mapView->scale(1.0, 1.0);

    ui->mapView->setObjectName("DialogMapView");

    if (parent != NULL) {
        resize(parent->width() * 0.6, parent->height() * 0.6);
    }
}

void MapDialog::setBackgroundBrush(QBrush brush) {
    ui->mapView->setBackgroundBrush(brush);
}

void MapDialog::scaleView(qreal step) {
    qreal scale = ui->mapView->transform().scale(1, 1).mapRect(QRectF(0, 0, 1, 1)).width();

    if ((step < 0 && scale <= 0.5) || (step > 0 && scale >= 5)) return;

    qreal scaleFactor = (scale + step) / (qreal)scale;

    ui->mapView->scale(scaleFactor, scaleFactor);

    QString zoomLabel = QString("%1x").arg(scale * scaleFactor, 0, 'f', 2);
    ui->zoomLabel->setText(zoomLabel);
}

void MapDialog::zoomIn() {
    scaleView(0.25);
}

void MapDialog::zoomOut() {
    scaleView(-0.25);
}

void MapDialog::reset() {
    RoomNode roomNode = mapFacade->getData()->getRoom();
    QString currentZoneId = roomNode.getZoneId();
    if(!currentZoneId.isEmpty()) {
        setSelected(currentZoneId, roomNode.getLevel());
        showMap(currentZoneId, roomNode.getLevel());
        center(roomNode);
        mapFacade->selectNode(roomNode.getZoneId(), roomNode.getLevel(), roomNode.getNodeId());
    }
}

void MapDialog::populate() {
    ui->levelSelect->setCurrentIndex(-1);
    ui->mapSelect->setCurrentIndex(-1);
    ui->mapSelect->clear();
    ui->mapSelect->addItem("");    

    QMap<QString, MapZone*> zones = mapFacade->getMapReader()->getZones();

    QMap<QString, MapZone*>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {
        ui->mapSelect->addItem(i.key() + ": " + i.value()->getName(), i.key());
    }
}

void MapDialog::mapSelected(int index) {    
    QString zoneId = ui->mapSelect->itemData(index).toString();
    this->populateLevels(zoneId);
    this->showMap(zoneId);
}

void MapDialog::setSelected(QString zoneId, int level) {
    int index = ui->mapSelect->findData(zoneId);
    if (index != -1) {
        this->populateLevels(zoneId, level);
        ui->mapSelect->setCurrentIndex(index);
    }
}

void MapDialog::populateLevels(QString zoneId, int level) {
    MapZone* zone = mapFacade->getMapReader()->getZones().value(zoneId);
    QList<int>& levels = zone->getLevels();
    if(zone != NULL) {
        ui->levelSelect->clear();
        foreach(int level, levels) {
            ui->levelSelect->addItem(QString::number(level), level);
        }        
        int index = ui->levelSelect->findData(level);
        if (index != -1) ui->levelSelect->setCurrentIndex(index);
    }
}

void MapDialog::mapLevelSelected(int index) {
    this->showMap(ui->mapSelect->currentData().toString(), ui->levelSelect->itemData(index).toInt());
}

void MapDialog::showMap(QString zoneId, int level) {
    ui->nodeInfo->clear();
    ui->mapView->setScene(mapFacade->getMapReader()->getScenes().value(zoneId).value(level).scene);
}

void MapDialog::center(const RoomNode& roomNode) {
    MapZone* zone = mapFacade->getMapReader()->getZones().value(roomNode.getZoneId());
    MapNode* node = zone->getNodes().value(roomNode.getNodeId());
    if(zone != NULL && node != NULL) {
        ui->mapView->centerOn(node->getPosition().getX() + abs(zone->getXMin()),
                              node->getPosition().getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN);
    }
}

void MapDialog::setInfo(MapNode* node) {
    QString info;
    info += "<b>[" + QString::number(node->getId()) + "]</b> " + node->getName();
    if(!node->getNotes().isEmpty()) {
        info += " (" + node->getNotes().join(", ") + ")";
    }
    info += "<br/>";

    for(int i = 0; i < node->getDesc().size(); i++) {
        info += QString::number(i) + ") " + node->getDesc().value(i) + "<br/>";
    }
    info += "<b>Exits:</b> ";
    QList<MapDestination*> values = node->getDestinations().values();
    for (int i = 0; i < values.size(); ++i) {
        info += "[" + values.at(i)->getMove() + " -> " + QString::number(values.at(i)->getDestId()) + "]; ";
    }
    ui->nodeInfo->setText(info);
}

void MapDialog::lockControls() {
    ui->mapSelect->setDisabled(true);
    ui->levelSelect->setDisabled(true);
}

void MapDialog::unlockControls() {
    ui->mapSelect->setDisabled(false);
    ui->levelSelect->setDisabled(false);
}

MapDialog::~MapDialog() {
    delete ui;
}
