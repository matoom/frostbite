#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QObject>

#include <maps/mapposition.h>

class MapLabel {

public:
    explicit MapLabel();
    MapLabel(const QString& text);

    const QString& getText() const;
    void setText(const QString& text);
    const MapPosition& getPosition() const;
    void setPosition(const MapPosition& position);

private:
    QString text;
    MapPosition position;

};

#endif // MAPLABEL_H
