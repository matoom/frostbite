#include "mapnode.h"

MapNode::MapNode() {

}

MapNode::MapNode(int id, QString name, QStringList notes, QString color) {
    this->id = id;
    this->name = name;
    this->notes = notes;
    this->color = color;
}

int MapNode::getId() {
    return this->id;
}

void MapNode::setId(int id) {
    this->id = id;
}

QString MapNode::getName() {
    return this->name;
}

void MapNode::setName(QString name) {
    this->name = name;
}

QStringList& MapNode::getDesc() {
    return this->desc;
}

void MapNode::setDesc(QStringList desc) {
    this->desc = desc;
}

QStringList MapNode::getNotes() {
    return this->notes;
}

void MapNode::setNotes(QStringList notes) {
    this->notes = notes;
}

QString MapNode::getColor() {
    return this->color;
}

void MapNode::setColor(QString color) {
    this->color = color;
}

MapPosition* MapNode::getPosition() {
    return this->position;
}

void MapNode::setMapPosition(MapPosition* position) {
    this->position = position;
}

QMultiHash<int, MapDestination* >& MapNode::getDestinations() {
    return this->destinations;
}

void MapNode::setDestinations(QMultiHash<int, MapDestination* > destinations) {
    this->destinations = destinations;
}
