#include "mapfacade.h"

MapFacade::MapFacade(MainWindow *parent) : QObject(parent) {
    mainWindow = parent;
    mapWindowFactory = new MapWindowFactory(this);

    settings = GeneralSettings::getInstance();

    this->init();
}

void MapFacade::init() {
    mapWindow = mapWindowFactory->createWindow(DOCK_TITLE_MAP);    
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, mapWindow);

    mapDialog = new MapDialog(this, mainWindow);

    mapSelect = mapWindow->findChild<QComboBox*>(QString(DOCK_TITLE_MAP) + "Select");
    levelSelect = mapWindow->findChild<QComboBox*>(QString(DOCK_TITLE_MAP) + "LevelSelect");
    mapIdLabel = mapWindow->findChild<QLabel*>(QString(DOCK_TITLE_MAP) + "IdLabel");
    zoomLabel = mapWindow->findChild<QLabel*>(QString(DOCK_TITLE_MAP) + "ZoomLabel");
    mapView = mapWindow->findChild<MapWindow*>(QString(DOCK_TITLE_MAP)+ "View");    

    connect(mapView, SIGNAL(updateMapWindow(QString)), this, SLOT(updateMapWindow(QString)));

    mapReader = new MapReader(this);
    connect(mapReader, SIGNAL(ready()), this, SLOT(mapsReady()));

    mapData = mapReader->getMapData();
}

void MapFacade::mapsReady() {
    if(!mapReader->isInitialized()) {
        this->setNotFoundMessage();
        return;
    }    
    if(mapView->scene() != NULL) mapView->scene()->clear();

    QMap<QString, MapZone*> zones = mapReader->getZones();

    levelSelect->setCurrentIndex(-1);
    mapSelect->setCurrentIndex(-1);
    mapSelect->clear();
    mapSelect->addItem("");    

    QMap<QString, MapZone*>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {
        mapSelect->addItem(i.key() + ": " + i.value()->getName(), i.key());
    }

    this->unlockControls();

    mapDialog->populate();
}

void MapFacade::setNotFoundMessage() {
    QGraphicsScene* scene = new QGraphicsScene(0, 0, 0, 0, this);
    QGraphicsTextItem* text = scene->addText("No map files found in - \n" + mapReader->getDir().absolutePath());
    text->setDefaultTextColor(mapReader->getTextColor(settings->dockWindowBackground()));
    mapView->setScene(scene);
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
        int index = levelSelect->findData(level);
        if (index != -1) levelSelect->setCurrentIndex(index);
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
    mapView->viewport()->update();
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

void MapFacade::updateMapColors(QColor background) {
    QColor text = mapReader->getTextColor(background);

    mapView->setBackgroundBrush(QBrush(background));
    mapDialog->setBackgroundBrush(QBrush(background));

    QHashIterator<QString, QHash<int, MapGraphics>> zones(mapReader->getScenes());
    while (zones.hasNext()) {
        zones.next();
        QHashIterator<int, MapGraphics> levels(zones.value());
        while (levels.hasNext()) {
            levels.next();
            QGraphicsScene* scene = levels.value().scene;
            QList<QGraphicsItem*> items = scene->items();
            for(QGraphicsItem* item : items) {
                QString data = item->data(Qt::UserRole).toString();
                if(data != NULL) {
                    if (data == "text") {
                        QGraphicsTextItem* textItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
                        textItem->setDefaultTextColor(text);
                    }
                }
            }
        }
    }
}

void MapFacade::setZoom(QString zoomLevel) {
    this->zoomLabel->setText(zoomLevel);
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
    map.selected->setPos(node->getPosition().getX() + abs(zone->getXMin()) -4,
                         node->getPosition().getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN -4);
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

void MapFacade::lockControls() {
    mapSelect->setDisabled(true);
    levelSelect->setDisabled(true);
    mapDialog->lockControls();
    mainWindow->enableMapsMenu(false);
}

void MapFacade::unlockControls() {
    mapSelect->setDisabled(false);
    levelSelect->setDisabled(false);
    mapDialog->unlockControls();
    mainWindow->enableMapsMenu(true);
}

MapFacade::~MapFacade() {
}

