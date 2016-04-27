#include "mapdestination.h"

MapDestination::MapDestination() {

}

MapDestination::MapDestination(int destId, QString exit, QString move) {
    this->destId = destId;
    this->exit = exit;
    this->move = move;
}

int MapDestination::getDestId() {
    return this->destId;
}

void MapDestination::setDestId(int destId) {
    this->destId = destId;
}

QString MapDestination::getExit() {
    return this->exit;
}

void MapDestination::setExit(QString exit) {
    this->exit = exit;
}

QString MapDestination::getMove() {
    return this->move;
}

void MapDestination::setMove(QString move) {
    this->move = move;
}
