#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QObject>
#include <QLibrary>

class DataService : public QObject {
    Q_OBJECT

public:
    static DataService* Instance();
    ~DataService();

    typedef void (*AddExp)(const char*, const char*, const char*);
    typedef void (*RemoveExp)(const char*);
    typedef void (*Set_b)(bool);
    typedef void (*Set_s)(const char*);
    typedef void (*Set_i)(int);

    AddExp addExpField;
    RemoveExp removeExpField;

    Set_b setStanding;
    Set_b setSitting;
    Set_b setKneeling;
    Set_b setProne;
    Set_b setStunned;
    Set_b setBleeding;
    Set_b setHidden;
    Set_b setInvisible;
    Set_b setWebbed;
    Set_b setJoined;
    Set_b setDead;

    Set_s setInventory;
    Set_s setContainer;
    Set_s setWieldRight;
    Set_s setWieldRightNoun;
    Set_s setWieldLeft;
    Set_s setWieldLeftNoun;

    Set_i setHealth;
    Set_i setConcentration;
    Set_i setSpirit;
    Set_i setFatigue;

    Set_s setRoomTitle;
    Set_s setRoomDescription;
    Set_s setRoomObjects;
    Set_s setRoomPlayers;
    Set_s setRoomExits;

    Set_i setRt;

private:
    DataService(QObject *parent = 0);
    DataService(DataService const& copy);
    DataService& operator = (DataService const& copy);
    static DataService* m_pInstance;

    void loadDataLib();

    QLibrary* dataLib;

signals:

public slots:

};

#endif // DATASERVICE_H
