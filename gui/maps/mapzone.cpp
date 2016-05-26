#include "mapzone.h"

MapZone::MapZone() {
    this->xMax = -10000;
    this->xMin = 10000;
    this->yMax = -10000;
    this->yMin = 10000;
}

MapZone::MapZone(QString id, QString name) {
    this->id = id;
    this->name = name;

    this->xMax = -10000;
    this->xMin = 10000;
    this->yMax = -10000;
    this->yMin = 10000;
}

QString MapZone::getId() {
    return this->id;
}

void MapZone::setId(QString id) {
    this->id = id;
}

QString MapZone::getName() {
    return this->name;
}

void MapZone::setName(QString name) {
    this->name = name;
}

QString MapZone::getFile() {
    return this->file;
}

void MapZone::setFile(QString file) {
    this->file = file;
}

QHash<int, MapNode*>& MapZone::getNodes() {
    return this->nodes;
}

void MapZone::setNodes(QHash<int, MapNode*> nodes) {
    this->nodes = nodes;
}

QList<MapLabel* >& MapZone::getLabels() {
    return this->labels;
}

void MapZone::setLabels(QList<MapLabel* > labels) {
    this->labels = labels;
}

int MapZone::getXMax() {
    return this->xMax;
}

void MapZone::setXMax(int xMax) {
    this->xMax = xMax;
}

int MapZone::getYMax() {
    return this->yMax;
}

void MapZone::setYMax(int yMax) {
    this->yMax = yMax;
}

int MapZone::getXMin() {
    return this->xMin;
}

void MapZone::setXMin(int xMin) {
    this->xMin = xMin;
}

int MapZone::getYMin() {
    return this->yMin;
}

void MapZone::setYMin(int yMin){
    this->yMin = yMin;
}

QList<int>& MapZone::getLevels() {
    return levels;
}

void MapZone::setLevels(QList<int> levels){
    this->levels = levels;
}
