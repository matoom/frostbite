#ifndef MAPDESTINATION_H
#define MAPDESTINATION_H

#include <QObject>

class MapDestination {

public:
    MapDestination();
    MapDestination(int destId, QString exit, QString move);

    int getDestId();
    void setDestId(int destId);
    QString getExit();
    void setExit(QString exit);
    QString getMove();
    void setMove(QString move);

private:
    int destId;
    QString exit;
    QString move;
};

#endif // MAPDESTINATION_H
