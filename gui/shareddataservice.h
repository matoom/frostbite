#ifndef SHAREDDATASERVICE_H
#define SHAREDDATASERVICE_H

#include <QObject>
#include <QLibrary>
#include <QDebug>
#include <QDir>
#include <QApplication>

class SharedDataService : public QObject {
    Q_OBJECT

public:
    static SharedDataService* Instance();
    //~SharedDataService();

    //typedef void (*AddExp)(const char*, const char*, const char*);
    typedef void (*AddExp)(const char*, int, int);
    typedef void (*RemoveExp)(const char*);
    typedef void (*Set_b)(bool);
    typedef void (*Set_s)(const char*);
    typedef void (*Set_i)(int);

    bool isLoaded();

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

    Set_s setActiveSpells;

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
    SharedDataService(QObject *parent = 0);
    SharedDataService(SharedDataService const& copy);
    SharedDataService& operator = (SharedDataService const& copy);
    static SharedDataService* m_pInstance;

    void loadDataLib();

    QLibrary* dataLib;
    bool loaded;

signals:

public slots:

};

#endif // SHAREDDATASERVICE_H
