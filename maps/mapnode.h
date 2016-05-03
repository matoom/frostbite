#ifndef MAPNODE_H
#define MAPNODE_H

#include <QString>
#include <QHash>

#include <maps/mapposition.h>
#include <maps/mapdestination.h>

class MapNode {

public:
    MapNode();
    MapNode(int id, QString name, QStringList notes, QString color);

    int getId();
    void setId(int id);

    QString getName();
    void setName(QString name);

    QStringList& getDesc();
    void setDesc(QStringList desc);

    QStringList getNotes();
    void setNotes(QStringList notes);

    QString getColor();
    void setColor(QString color);

    MapPosition* getPosition();
    void setMapPosition(MapPosition* position);
    QMultiHash<int, MapDestination*>& getDestinations();
    void setDestinations(QMultiHash<int, MapDestination*> destinations);

private:
    int id;
    QString name;
    QStringList desc;
    QStringList notes;
    QString color;

    MapPosition* position;
    QMultiHash<int, MapDestination* > destinations;

};

#endif // MAPNODE_H
