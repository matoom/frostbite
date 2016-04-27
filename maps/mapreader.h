#ifndef MAPREADER_H
#define MAPREADER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QApplication>
#include <QDomDocument>
#include <QDir>
#include <QtGlobal>
#include <QPainter>
#include <QGraphicsScene>

#include <QtConcurrent>

#include <qdebug.h>
#include <maps/mapzone.h>
#include <maps/mapnode.h>
#include <maps/maplabel.h>
#include <maps/maprect.h>

#include <mainwindow.h>

#include <globaldefines.h>

struct RoomNode {
    QString zoneId;
    int nodeId;
    int level;
};

class MainWindow;

class MapReader : public QObject {
    Q_OBJECT

public:
    explicit MapReader(QObject *parent = 0);

    QMap<QString, MapZone*> getZones();

    QHash<QString, QHash<int, QGraphicsScene*> > getScenes();
    RoomNode findRoomNode(QString hash);

    void init();

private:
    MapZone* readZone(QString path, QString file);

    void paintArcs(MapZone* zone, QHash<int, QGraphicsScene*>& scenes);
    void paintLabels(MapZone* zone, QHash<int, QGraphicsScene*>& scenes);
    void paintNodes(MapZone* zone, QHash<int, QGraphicsScene*>& scenes);
    void paintEndpoint(MapZone* zone, MapNode* node, QGraphicsScene* scene);

    boolean isInRange(int n);
    void roomToHash();

    MainWindow* mainWindow;

    void paintScenes();
    QHash<int, QGraphicsScene*> paintScene(MapZone* zone);

    MapZone* mapZone;
    MapNode* mapNode;
    MapLabel* mapLabel;
    MapPosition* position;

    QMap<QString, MapZone*> zones;
    QHash<QString, QHash<int, QGraphicsScene*> > scenes;
    QHash<QString, QString> connections;

    QStringList ids;

    QMultiHash<QString, RoomNode> roomNodes;

signals:
    void ready();
    void readyRead();

public slots:
    void initScenes();
};

#endif // MAPREADER_H
