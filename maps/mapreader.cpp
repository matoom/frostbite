#include "mapreader.h"

MapReader::MapReader(QObject* parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    QtConcurrent::run(this, &MapReader::init);

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

void MapReader::initScenes() {
    this->paintScenes();
    emit ready();
}

QHash<QString, QHash<int, QGraphicsScene*> > MapReader::getScenes() {
    return this->scenes;
}

void MapReader::paintScenes() {
    QMap<QString, MapZone*>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {
        this->scenes.insert(i.key(), paintScene(i.value()));
    }
}

QHash<int, QGraphicsScene*> MapReader::paintScene(MapZone* zone) {
    int w  = zone->getXMax() + abs(zone->getXMin()) + 100;
    int h  = zone->getYMax() + abs(zone->getYMin()) + 25 + MAP_TOP_MARGIN;

    QHash<int, QGraphicsScene*> scenes;

    QList<int>& levels = zone->getLevels();
    foreach(int level, levels) {
            QGraphicsScene* scene = new QGraphicsScene(0, 0, w, h, mainWindow);
            scene->addText(zone->getName() + " (" + QString::number(level) + "/" +
                           QString::number(levels.size() - 1) + ")");
            scenes.insert(level, scene);
    }

    this->paintArcs(zone, scenes);
    this->paintLabels(zone, scenes);
    this->paintNodes(zone, scenes);

    return scenes;
}

void MapReader::paintArcs(MapZone* zone, QHash<int, QGraphicsScene*>& scenes) {
    foreach(MapNode* node, zone->getNodes()) {
        QGraphicsScene* scene = scenes.value(node->getPosition()->getZ());
        foreach(MapDestination* dest, node->getDestinations()) {
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

void MapReader::paintLabels(MapZone* zone, QHash<int, QGraphicsScene*>& scenes) {
    foreach(MapLabel* label, zone->getLabels()) {
        QGraphicsScene* scene = scenes.value(label->getPosition()->getZ());
        QGraphicsTextItem* textItem = scene->addText(label->getText());
        textItem->setPos(label->getPosition()->getX() + abs(zone->getXMin()),
                         label->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN);

    }
}

void MapReader::paintNodes(MapZone* zone, QHash<int, QGraphicsScene*>& scenes) {
    foreach(MapNode* node, zone->getNodes()) {
        QGraphicsScene* scene = scenes.value(node->getPosition()->getZ());

        QColor color;
        if(node->getColor() != NULL) {
           color = QColor(node->getColor());
        } else {
           color = Qt::lightGray;
        }

        MapRect* item = new MapRect(node->getPosition()->getX() + abs(zone->getXMin()),
                                    node->getPosition()->getY() + abs(zone->getYMin()) + MAP_TOP_MARGIN, 4, 4,
                                    QColor(Qt::darkGray), color);

        item->setZoneId(zone->getId());
        item->setNodeId(node->getId());
        connect(item, SIGNAL(nodeSelected(QString, int)), mainWindow->getWindowFacade(), SLOT(selectNode(QString, int)));

        foreach(QString note, node->getNotes()) {
            if(note.endsWith(".xml")) {
                QString endNode = connections.value(note);
                if(endNode != NULL) {
                    item->setEndNode(endNode);
                    this->paintEndpoint(zone, node, scene);
                    connect(item, SIGNAL(go(QString, int)), mainWindow->getWindowFacade(), SLOT(showMap(QString, int)));
                }
                break;
            }
        }
        scene->addItem(item);
    }
}

void MapReader::paintEndpoint(MapZone* zone, MapNode* node, QGraphicsScene* scene) {
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
                mapNode = new MapNode(xml.attributes().value("id").toInt(),
                                      xml.attributes().value("name").toString(),
                                      xml.attributes().value("note").toString().split("|"),
                                      xml.attributes().value("color").toString());
            } else {                                                
                mapNode->setMapPosition(position);                
                mapZone->getNodes().insert(mapNode->getId(), mapNode);

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
            dest->setDestId(attr.value("destination").toInt());
            dest->setMove(attr.value("move").toString());
            dest->setExit(attr.value("exit").toString());

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

        /*if(mapNode->getId() == 20) {
            qDebug() << text;
            qDebug() << hash;
        }*/

        roomNodes.insert(hash, {mapZone->getId(), mapNode->getId(), mapNode->getPosition()->getZ()});
    }
}

boolean MapReader::isInRange(int n) {
    return n < 4000 && n > -4000;
}

RoomNode MapReader::findRoomNode(QString hash) {
   return roomNodes.value(hash);
}

QMap<QString, MapZone*> MapReader::getZones() {
    return this->zones;
}
