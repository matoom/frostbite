#include "mapfacade.h"

MapFacade::MapFacade(MainWindow *parent) : QObject(parent) {
    mainWindow = parent;
    mapWindowFactory = new MapWindowFactory(this);

    this->init();
}

void MapFacade::init() {
    mapWindow = mapWindowFactory->createWindow(DOCK_TITLE_MAP);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, mapWindow);

    mapDialog = new MapDialog(this, mainWindow);

    mapSelect = mapWindow->findChild<QComboBox*>(QString(DOCK_TITLE_MAP) + "Select");
    levelSelect = mapWindow->findChild<QComboBox*>(QString(DOCK_TITLE_MAP) + "LevelSelect");
    mapIdLabel = mapWindow->findChild<QLabel*>(QString(DOCK_TITLE_MAP) + "IdLabel");
    mapView = mapWindow->findChild<MapWindow*>(QString(DOCK_TITLE_MAP)+ "View");

    mapReader = new MapReader(this);
    connect(mapReader, SIGNAL(ready()), this, SLOT(mapsReady()));

    mapData = mapReader->getMapData();
}

void MapFacade::mapsReady() {
    if(!mapReader->isInitialized()) return;

    QMap<QString, MapZone*> zones = mapReader->getZones();

    mapSelect->addItem("");

    QMap<QString, MapZone*>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {
        mapSelect->addItem(i.key() + ": " + i.value()->getName(), i.key());
    }

    mapSelect->setDisabled(false);
    levelSelect->setDisabled(false);

    mapDialog->populate();
}

MapData* MapFacade::getData() {
    return mapReader->getMapData();
}

void MapFacade::updateMapWindow(QString hash) {
    if(!mapReader->isInitialized()) return;

    RoomNode node = mapData->findRoomNode(hash);

    mapData->setRoom(node);
    if(!node.getZoneId().isEmpty()) {
        this->setSelected(node.getZoneId(), node.getLevel());
        this->showMap(node.getZoneId(), node.getLevel());
        this->selectNode(node.getZoneId(), node.getLevel(), node.getNodeId());
    }
}

void MapFacade::mapSelected(int index) {
    QString zoneId = mapSelect->itemData(index).toString();
    this->populateLevels(zoneId);
    this->showMap(zoneId);
}

void MapFacade::setSelected(QString zoneId, int level) {
    int index = mapSelect->findData(zoneId);
    if (index != -1) {
        this->populateLevels(zoneId, level);
        mapSelect->setCurrentIndex(index);
    }
}

void MapFacade::populateLevels(QString zoneId, int level) {
    MapZone* zone = mapReader->getZones().value(zoneId);
    QList<int>& levels = zone->getLevels();
    if(zone != NULL) {
        levelSelect->clear();
        foreach(int level, levels) {
            levelSelect->addItem(QString::number(level), level);
        }
        if(level != 0){
            int index = levelSelect->findData(level);
            if (index != -1) levelSelect->setCurrentIndex(index);
        }
    }
}

void MapFacade::mapLevelSelected(int index) {
    this->showMap(mapSelect->currentData().toString(), levelSelect->itemData(index).toInt());
}

void MapFacade::showMap(QWidget* widget, QString zoneId, int level) {
    QGraphicsView* view = (QGraphicsView*)widget->parent();
    QString obj = view->objectName();

    if(obj == "MapView") {
        this->setSelected(zoneId);
        this->showMap(zoneId, level);
    } else if(obj == "DialogMapView") {
        mapDialog->setSelected(zoneId, level);
        mapDialog->showMap(zoneId, level);
    }
}

void MapFacade::showMap(QString zoneId, int level) {
    mapView->setScene(mapReader->getScenes().value(zoneId).value(level).scene);
}

void MapFacade::selectNode(QWidget* widget, QString zoneId, int level, int nodeId) {
    QString obj = widget->parent()->objectName();

    if(obj == "MapView") {
        mapData->setRoom(RoomNode(zoneId, level, nodeId));
        this->selectNode(zoneId, level, nodeId);
    } else if (obj == "DialogMapView") {
        this->moveSelected(zoneId, nodeId, level);

        MapZone* zone = mapReader->getZones().value(zoneId);
        MapNode* node = zone->getNodes().value(nodeId);

        mapDialog->setInfo(node);
    }
}

void MapFacade::selectNode(QString zoneId, int level, int nodeId) {
    mapIdLabel->setText(QString::number(nodeId));
    this->moveSelected(zoneId, nodeId, level);
    emit nodeSelected(mapReader->getZones().value(zoneId), nodeId);
}

void MapFacade::moveSelected(QString zoneId, int nodeId, int level) {
    MapZone* zone = mapReader->getZones().value(zoneId);
    MapNode* node = zone->getNodes().value(nodeId);

    MapGraphics map = mapReader->getScenes().value(zoneId).value(level);
    map.selected->setPos(node->getPosition()->getX() + abs(zone->getXMin()) -4,
                         node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN -4);
    map.selected->show();
}

void MapFacade::showMapDialog() {
    if(!mapReader->isInitialized()) return;

    QString currentZoneId = mapSelect->currentData().toString();
    if(!currentZoneId.isEmpty()) {
        mapDialog->setSelected(currentZoneId, levelSelect->currentData().toInt());
        mapDialog->showMap(currentZoneId, levelSelect->currentData().toInt());
    }
    mapDialog->show();
}

MapReader* MapFacade::getMapReader() {
    return this->mapReader;
}

QDockWidget* MapFacade::getMapWindow() {
    return this->mapWindow;
}

MainWindow* MapFacade::getMainWindow() {
    return this->mainWindow;
}


