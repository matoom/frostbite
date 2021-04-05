#include "maplabel.h"

MapLabel::MapLabel() {

}

MapLabel::MapLabel(const QString& text) {
    this->text = text;
}

const QString& MapLabel::getText() const {
    return this->text;
}

void MapLabel::setText(const QString& text) {
    this->text = text;
}

const MapPosition& MapLabel::getPosition() const {
    return this->position;
}

void MapLabel::setPosition(const MapPosition& position) {
    this->position = position;
}

