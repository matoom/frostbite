#include "wieldmodel.h"

WieldModel::WieldModel(QObject *parent) : QObject(parent) {
}

QString WieldModel::toString() {
    return "WieldModel:[ left => " + this->left  +
           ", right => " + this->right  + " ]";
}
