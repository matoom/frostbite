#include "roommodel.h"

RoomModel::RoomModel(QObject *parent) : QObject(parent) {

}

QString RoomModel::toString() {
    return "ExpModel:[ desc => " + this->desc.left(10)  +
           ", objs => " + this->objs.left(10)  +
           ", players => " + this->players.left(10) + " ]";
}
