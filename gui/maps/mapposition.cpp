#include "mapposition.h"

MapPosition::MapPosition() {
}

MapPosition::MapPosition(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

int MapPosition::getX() {
    return this->x;
}

void MapPosition::setX(int x) {
    this->x = x;
}

int MapPosition::getY() {
    return this->y;
}

void MapPosition::setY(int y) {
    this->y = y;
}

int MapPosition::getZ() {
    return this->z;
}

void MapPosition::setZ(int z) {
    this->z = z;
}
