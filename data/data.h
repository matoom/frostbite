#ifndef DATA_H
#define DATA_H

#include "data_global.h"

#include <QString>
#include <QDebug>

#ifdef Q_WS_WIN
#define EXPORT_FUNCTION __declspec(dllexport)
#else
#define EXPORT_FUNCTION
#endif

class DATASHARED_EXPORT Data {

public:
    Data();

    /* exp */
    static const int expRows = 100;
    static const int expY = 3;
    static const int expZ = 18;

    void addExpField(const char[], const char[], const char[]);
    void removeExpField(const char[]);

    int getExpIndex(const char[]);
    int getExpRank(const char[]);
    int getExpState(const char[]);

    /* status getters/setters */
    bool getStanding();
    void setStanding(bool);
    bool getSitting();
    void setSitting(bool);
    bool getKneeling();
    void setKneeling(bool);
    bool getProne();
    void setProne(bool);
    bool getStunned();
    void setStunned(bool);
    bool getBleeding();
    void setBleeding(bool);
    bool getHidden();
    void setHidden(bool);
    bool getInvisible();
    void setInvisible(bool);
    bool getWebbed();
    void setWebbed(bool);
    bool getJoined();
    void setJoined(bool);
    bool getDead();
    void setDead(bool);

    /* inventory */
    static const int inventorySize = 8192;
    char* getInventory();
    void setInventory(const char[]);

    /* container */
    static const int containerSize = 8192;
    char* getContainer();
    void setContainer(const char[]);

    /* wield */
    static const int wieldSize = 50;
    char* getWieldRight();
    void setWieldRight(const char[]);
    char* getWieldRightNoun();
    void setWieldRightNoun(const char[]);
    char* getWieldLeft();
    void setWieldLeft(const char[]);
    char* getWieldLeftNoun();
    void setWieldLeftNoun(const char[]);

    /* vitals */
    int getHealth();
    void setHealth(int);
    int getConcentration();
    void setConcentration(int);
    int getSpirit();
    void setSpirit(int);
    int getFatigue();
    void setFatigue(int);

    /* room */
    static const int roomTitleSize = 256;
    static const int roomDescriptionSize = 4096;
    static const int roomObjectsSize = 8192;
    static const int roomPlayersSize = 8192;
    static const int roomExitsSize = 256;

    char* getRoomTitle();
    void setRoomTitle(const char[]);
    char* getRoomDescription();
    void setRoomDescription(const char[]);
    char* getRoomObjects();
    void setRoomObjects(const char[]);
    char* getRoomPlayers();
    void setRoomPlayers(const char[]);
    char* getRoomExits();
    void setRoomExits(const char[]);

    /* roundtime */
    int getRt();
    void setRt(int);

private:
    int getLastFreeIndex();
    void setExp(int, const char[], const char[], const char[]);
    void removeExp(int index);
};

#endif // DATA_H
