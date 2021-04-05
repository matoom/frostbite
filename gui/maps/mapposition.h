#ifndef MAPPOSITION_H
#define MAPPOSITION_H

class MapPosition {

public:
    MapPosition(int x = 0, int y = 0, int z = 0);

    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);
    int getZ() const;
    void setZ(int z);

private:
    int x;
    int y;
    int z;
};

#endif // MAPPOSITION_H
