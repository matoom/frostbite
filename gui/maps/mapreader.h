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

#include <maps/mapfacade.h>
#include <maps/mapdata.h>
#include <maps/roomnode.h>

#include <globaldefines.h>

struct MapGraphics {
    QGraphicsScene* scene;
    QGraphicsEllipseItem* selected;
};

class MapFacade;
class MapData;

class MapReader : public QObject {
    Q_OBJECT

public:
    explicit MapReader(QObject *parent = 0);
    ~MapReader();

    QMap<QString, MapZone*> getZones();
    QHash<QString, QHash<int, MapGraphics> > getScenes();
    QMultiHash<QString, RoomNode> getRoomNodes();

    MapData* getMapData();
    QHash<QString, RoomNode> getLocations();

    QDir getDir();

    void init();    

    bool isInitialized();

    QColor getTextColor(QColor background);
    QColor getLineColor();

private:
    MapZone* readZone(QString path, QString file);

    void paintArcs(MapZone* zone, QHash<int, MapGraphics>& scenes);
    void paintLabels(MapZone* zone, QHash<int, MapGraphics>& scenes);
    void paintNodes(MapZone* zone, QHash<int, MapGraphics>& scenes);
    void paintEndNode(MapZone* zone, MapNode* node, QGraphicsScene* scene);

    bool isInRange(int n);
    void roomToHash();

    void setInitialized(boolean initialized);

    void uninit();
    void clear();

    QDir dir;
    QFont labelsFont;

    bool initialized;

    MapFacade* mapFacade;
    MapData* mapData;

    void paintScenes();
    QHash<int, MapGraphics> paintScene(MapZone* zone);

    MapZone* mapZone;
    MapNode* mapNode;
    MapLabel* mapLabel;
    MapPosition position;

    QMap<QString, MapZone*> zones;
    QHash<QString, QHash<int, MapGraphics> > scenes;
    QHash<QString, QString> connections;
    QHash<QString, RoomNode> locations;

    QStringList ids;

    QMultiHash<QString, RoomNode> roomNodes;

    QReadWriteLock lock;

    GeneralSettings* settings;
    QColor background;

    QFutureWatcher<void> reloadWatcher;

signals:
    void ready();
    void readyRead();

public slots:
    void initScenes();
    void reload();
    void concurrentInit();
};

#endif // MAPREADER_H
