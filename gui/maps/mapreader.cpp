#include "mapreader.h"

MapReader::MapReader(QObject* parent) : QObject(parent) {
    mapFacade = (MapFacade*)parent;
    initialized = false;

    QtConcurrent::run(this, &MapReader::init);

    mapData = new MapData(this);

    connect(this, SIGNAL(readyRead()), this, SLOT(initScenes()));
}

void MapReader::init() {
    QDir dir(QApplication::applicationDirPath() + "/maps");

    QStringList filter;
    filter << "*.xml";

    QStringList fileList = dir.entryList(filter, QDir::Files, QDir::Name);

    foreach(QString file, fileList) {        
        MapZone* zone = readZone(dir.path(), file);
        zones.insert(zone->getId(), zone);
    }    
    emit readyRead();
}

void MapReader::setInitialized() {
    QWriteLocker locker(&lock);
    this->initialized = true;
}

bool MapReader::isInitialized() {
    QReadLocker locker(&lock);
    return this->initialized;
}

void MapReader::initScenes() {
    this->paintScenes();
    emit ready();
}

QHash<QString, QHash<int, MapGraphics> > MapReader::getScenes() {
    QReadLocker locker(&lock);
    return this->scenes;
}

QMap<QString, MapZone*> MapReader::getZones() {
    QReadLocker locker(&lock);
    return this->zones;
}

QMultiHash<QString, RoomNode> MapReader::getRoomNodes() {
    QReadLocker locker(&lock);
    return this->roomNodes;
}

QHash<QString, RoomNode> MapReader::getLocations() {
    QReadLocker locker(&lock);
    return this->locations;
}

MapData* MapReader::getMapData() {
    QReadLocker locker(&lock);
    return mapData;
}

void MapReader::paintScenes() {
    QMap<QString, MapZone*>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {   
        this->scenes.insert(i.key(), paintScene(i.value()));
    }
    if (!zones.isEmpty()) this->setInitialized();
}

QHash<int, MapGraphics> MapReader::paintScene(MapZone* zone) {
    int w  = zone->getXMax() + abs(zone->getXMin()) + 100;
    int h  = zone->getYMax() + abs(zone->getYMin()) + 25 + MAP_TOP_MARGIN;

    QHash<int, MapGraphics> scenes;

    QList<int>& levels = zone->getLevels();
    foreach(int level, levels) {
            QGraphicsScene* scene = new QGraphicsScene(0, 0, w, h, mapFacade);
            scene->setObjectName(zone->getId());
            scene->addText(zone->getName() + " (" + QString::number(level) + "/" +
                           QString::number(levels.size() - 1) + ")");

            QGraphicsEllipseItem* selected = scene->addEllipse(0, 0, 12, 12,  QColor("red"));
            selected->hide();

            scenes.insert(level, {scene, selected});
    }

    this->paintArcs(zone, scenes);
    this->paintLabels(zone, scenes);
    this->paintNodes(zone, scenes);

    return scenes;
}

void MapReader::paintArcs(MapZone* zone, QHash<int, MapGraphics>& scenes) {
    foreach(MapNode* node, zone->getNodes()) {
        QGraphicsScene* scene = scenes.value(node->getPosition()->getZ()).scene;
        foreach(MapDestination* dest, node->getDestinations()) {
            if(!dest->getHidden()) {
                MapNode* destNode = zone->getNodes().value(dest->getDestId());
                if(destNode != NULL) {
                    scene->addLine(node->getPosition()->getX() + abs(zone->getXMin()) + 2,
                                   node->getPosition()->getY() + abs(zone->getYMin()) + 2 + MAP_TOP_MARGIN,
                                   destNode->getPosition()->getX() + abs(zone->getXMin()) + 2,
                                   destNode->getPosition()->getY() + abs(zone->getYMin()) + 2 + MAP_TOP_MARGIN,
                                   QColor(Qt::darkGray));
                }
            }
        }
    }
}

void MapReader::paintLabels(MapZone* zone, QHash<int, MapGraphics>& scenes) {
    foreach(MapLabel* label, zone->getLabels()) {
        QGraphicsScene* scene = scenes.value(label->getPosition()->getZ()).scene;
        QGraphicsTextItem* textItem = scene->addText(label->getText());
        textItem->setPos(label->getPosition()->getX() + abs(zone->getXMin()),
                         label->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN);
    }
}

void MapReader::paintNodes(MapZone* zone, QHash<int, MapGraphics>& scenes) {
    foreach(MapNode* node, zone->getNodes()) {
        QGraphicsScene* scene = scenes.value(node->getPosition()->getZ()).scene;

        QColor color;
        if(node->getColor() != NULL) {
           color = QColor(node->getColor());
        } else {
           color = Qt::lightGray;
        }

        MapRect* item = new MapRect(node->getPosition()->getX() + abs(zone->getXMin()),
                                    node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN,
                                    4, 4,
                                    QColor(Qt::darkGray), color);

        item->setZoneId(zone->getId());
        item->setNodeId(node->getId());
        item->setLevel(node->getPosition()->getZ());
        item->setScene(scene);

        connect(item, SIGNAL(nodeSelected(QWidget*, QString, int, int)), mapFacade, SLOT(selectNode(QWidget*, QString, int, int)));

        foreach(QString note, node->getNotes()) {
            if(note.endsWith(".xml")) {
                QString endZoneId = connections.value(note);
                if(endZoneId != NULL) {
                    item->setEndZone(endZoneId);
                    this->paintEndNode(zone, node, scene);
                    connect(item, SIGNAL(go(QWidget*, QString, int)), mapFacade, SLOT(showMap(QWidget*, QString, int)));
                }
                break;
            }
        }
        scene->addItem(item);
    }
}

void MapReader::paintEndNode(MapZone* zone, MapNode* node, QGraphicsScene* scene) {
    scene->addEllipse(node->getPosition()->getX() + abs(zone->getXMin()) -4,
                      node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN -4,
                      12, 12,  QColor(0, 170, 255, 255));
}

MapZone* MapReader::readZone(QString path, QString file) {
    QFile xmlFile(path + "/" + file);
    xmlFile.open(QIODevice::ReadOnly);

    QXmlStreamReader xml;
    xml.setDevice(&xmlFile);

    while(!xml.atEnd()) {
        if (xml.tokenType() == QXmlStreamReader::Invalid) {
            xml.readNext();
        }

        xml.readNext();

        if(xml.name() == "zone" && xml.isStartElement()) {
            QString id = xml.attributes().value("id").toString();
            ids << id;
            int count = ids.count(id);
            if(count > 1) id += QChar(count + 96);
            connections.insert(file, id);
            mapZone = new MapZone(id, xml.attributes().value("name").toString());
            mapZone->setFile(file);            
        } else if (xml.name() == "node") {
            if(xml.isStartElement()) {
                QString note = xml.attributes().value("note").toString();

                QStringList notes;
                if(!note.isEmpty()) notes = note.split("|");

                mapNode = new MapNode(xml.attributes().value("id").toInt(), xml.attributes().value("name").toString(),
                                      notes, xml.attributes().value("color").toString());
            } else {                                                
                mapNode->setMapPosition(position);                
                mapZone->getNodes().insert(mapNode->getId(), mapNode);

                foreach(QString note, mapNode->getNotes()) {
                    locations.insert(note, RoomNode(mapZone->getId(), mapNode->getPosition()->getZ(), mapNode->getId()));
                }

                this->roomToHash();
            }
        } else if (xml.name() == "description" && xml.isStartElement()) {
            mapNode->getDesc().append(xml.readElementText());
        } else if (xml.name() == "position" && xml.isStartElement()) {
            QXmlStreamAttributes attr = xml.attributes();
            int x = attr.value("x").toInt();
            int y = attr.value("y").toInt();
            int z = attr.value("z").toInt();

            if(isInRange(x) && isInRange(y)) {
                if(x > mapZone->getXMax()) mapZone->setXMax(x);
                if(y > mapZone->getYMax()) mapZone->setYMax(y);

                if(x < mapZone->getXMin()) mapZone->setXMin(x);
                if(y < mapZone->getYMin()) mapZone->setYMin(y);

                if(!mapZone->getLevels().contains(z)) mapZone->getLevels() << z;

                position = new MapPosition(x, y, z);
            }
        } else if(xml.name() == "arc" && xml.isStartElement()) {
            QXmlStreamAttributes attr = xml.attributes();

            MapDestination* dest = new MapDestination();
            if(attr.hasAttribute("destination")) {
                dest->setDestId(attr.value("destination").toInt());
            } else {
                dest->setDestId(-1);
            }
            dest->setMove(attr.value("move").toString());
            dest->setExit(attr.value("exit").toString());
            QString hidden = attr.value("hidden").toString();
            if(hidden.toLower() == "true") {
                dest->setHidden(true);
            } else {
                dest->setHidden(false);
            }
            mapNode->getDestinations().insert(attr.value("destination").toInt(), dest);
        } else if(xml.name() == "label") {
            if(xml.isStartElement()) {
                mapLabel = new MapLabel(xml.attributes().value("text").toString());
            } else {
                mapLabel->setPosition(position);
                mapZone->getLabels().append(mapLabel);
            }
        }
    }

    qSort(mapZone->getLevels());

    return mapZone;
}

void MapReader::roomToHash() {
    QMultiHash<int, MapDestination* >& dest = mapNode->getDestinations();

    QList<QString> list;

    QHash<int, MapDestination*>::iterator i;
    for (i = dest.begin(); i != dest.end(); ++i) {
        QString d = TextUtils::Instance()->toBrief(i.value()->getExit());
        if(!d.isEmpty()) list << d;
    }
    qSort(list);

    QStringList descList = mapNode->getDesc();
    foreach(QString desc, descList) {
        TextUtils::Instance()->plainToHtml(desc);

        QString text = "[" + mapNode->getName() + "]" + desc + list.join("");
        QString hash = TextUtils::Instance()->toHash(text);

        /*if(mapNode->getId() == 100) {
            qDebug() << text;
            qDebug() << hash;
        }*/        

        int level = mapNode->getPosition()->getZ();
        int nodeId = mapNode->getId();

        roomNodes.insert(hash, RoomNode(mapZone->getId(), level, nodeId));
    }
}

bool MapReader::isInRange(int n) {
    return n < 4000 && n > -4000;
}

