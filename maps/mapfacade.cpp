#include "mapfacade.h"

MapFacade::MapFacade(MainWindow *parent) : QObject(parent) {
    mainWindow = parent;
    mapWindowFactory = new MapWindowFactory(this);
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

void MapFacade::updateMapWindow(QString hash) {
    if(!mapReader->isInitialized()) return;

    RoomNode node = mapReader->findRoomNode(hash);

    if(!node.zoneId.isEmpty()) {
        int index = mapSelect->findData(node.zoneId);
        if (index != -1) mapSelect->setCurrentIndex(index);

        QList<int>& levels = mapReader->getZones().value(node.zoneId)->getLevels();
        foreach(int level, levels) {
            levelSelect->addItem(QString::number(level), level);
        }

        index = levelSelect->findData(node.level);
        if (index != -1) levelSelect->setCurrentIndex(index);

        this->showMap(node.zoneId, node.level);
        this->selectNode(node.zoneId, node.level, node.nodeId);
    }
}

void MapFacade::mapSelected(int index) {
    QString id = mapSelect->itemData(index).toString();

    QList<int>& levels = mapReader->getZones().value(id)->getLevels();

    levelSelect->clear();
    foreach(int level, levels) {
        levelSelect->addItem(QString::number(level), level);
    }
    this->showMap(id);
}

void MapFacade::mapLevelSelected(int index) {
    this->showMap(mapSelect->currentData().toString(), levelSelect->itemData(index).toInt());
}

void MapFacade::showMap(QWidget* widget, QString zoneId, int level) {
    int index = mapSelect->findData(zoneId);
    if (index != -1) mapSelect->setCurrentIndex(index);

    QGraphicsView* view = (QGraphicsView*)widget->parent();
    QGraphicsScene* scene = mapReader->getScenes().value(zoneId).value(level).scene;

    view->setScene(scene);
}

void MapFacade::showMap(QString zoneId, int level) {
    int index = mapSelect->findData(zoneId);
    if (index != -1) mapSelect->setCurrentIndex(index);

    QGraphicsScene* scene = mapReader->getScenes().value(zoneId).value(level).scene;
    mapView->setScene(scene);
}

void MapFacade::selectNode(QWidget* widget, QString zoneId, int level, int nodeId) {
    QString obj = widget->parent()->objectName();

    if(obj == "MapView") {
        this->selectNode(zoneId, level, nodeId);
    } else if (obj == "DialogMapView") {
        this->moveSelected(zoneId, nodeId, level);

        MapZone* zone = mapReader->getZones().value(zoneId);
        MapNode* node = zone->getNodes().value(nodeId);
        mapDialog->setInfo(node);
    }
}

void MapFacade::selectNode(QString zoneId, int level, int nodeId) {
    mapIdLabel->setText("Id: " + QString::number(nodeId));
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

    QString currentMap = mapSelect->currentData().toString();
    if(!currentMap.isEmpty()) {
        mapDialog->showMap(currentMap, levelSelect->currentData().toInt());
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


