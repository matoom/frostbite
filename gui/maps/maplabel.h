#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QObject>

#include <maps/mapposition.h>

class MapLabel {

public:
    MapLabel();
    MapLabel(QString text);

    QString getText();
    void setText(QString text);
    MapPosition getPosition();
    void setPosition(MapPosition position);

private:
    QString text;
    MapPosition position;

};

#endif // MAPLABEL_H
