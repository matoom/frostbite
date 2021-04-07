#include "mapposition.h"

MapPosition::MapPosition(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

int MapPosition::getX() const {
    return this->x;
}

void MapPosition::setX(int x) {
    this->x = x;
}

int MapPosition::getY() const {
    return this->y;
}

void MapPosition::setY(int y) {
    this->y = y;
}

int MapPosition::getZ() const {
    return this->z;
}

void MapPosition::setZ(int z) {
    this->z = z;
}
