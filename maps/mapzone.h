#ifndef MAPZONE_H
#define MAPZONE_H

#include <QObject>
#include <QHash>
#include <QSet>

#include <maps/mapnode.h>
#include <maps/maplabel.h>

class MapZone {

public:
    MapZone();
    MapZone(QString id, QString name);

    QString getId();
    void setId(QString id);

    QString getName();
    void setName(QString name);

    QString getFile();
    void setFile(QString file);

    QHash<int, MapNode* >& getNodes();
    void setNodes(QHash<int, MapNode* > nodeMap);

    QList<MapLabel* >& getLabels();
    void setLabels(QList<MapLabel* > nodeMap);

    int getXMax();
    void setXMax(int xMax);
    int getYMax();
    void setYMax(int yMax);

    int getXMin();
    void setXMin(int xMin);
    int getYMin();
    void setYMin(int yMin);

    QList<int>& getLevels();
    void setLevels(QList<int> levels);

private:
    QString id;
    QString name;
    QString file;
    QHash<int, MapNode* > nodes;
    QList<MapLabel* > labels;

    int xMax;
    int yMax;

    int xMin;
    int yMin;

    QList<int> levels;

};

#endif // MAPZONE_H
