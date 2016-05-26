#ifndef MAPPOSITION_H
#define MAPPOSITION_H

class MapPosition {

public:
    MapPosition();
    MapPosition(int x, int y, int z);

    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    int getZ();
    void setZ(int z);

private:
    int x;
    int y;
    int z;
};

#endif // MAPPOSITION_H
