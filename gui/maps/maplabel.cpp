#include "maplabel.h"

MapLabel::MapLabel() {

}

MapLabel::MapLabel(QString text) {
    this->text = text;
}

QString MapLabel::getText() {
    return this->text;
}

void MapLabel::setText(QString text) {
    this->text = text;
}

MapPosition* MapLabel::getPosition() {
    return this->position;
}

void MapLabel::setPosition(MapPosition* position) {
    this->position = position;
}

