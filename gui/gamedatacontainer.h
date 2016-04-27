#ifndef GAMEDATACONTAINER_H
#define GAMEDATACONTAINER_H

#include <QObject>
#include <QHash>
#include <QReadLocker>

#include <textutils.h>
//#include <shareddataservice.h>

class TextUtils;
//class SharedDataService;

class GameDataContainer : public QObject {
    Q_OBJECT

public:
    static GameDataContainer* Instance();

    void setExpField(QString, QString);
    void setExpFieldBrief(QString, QString);

    void setContainer(QStringList);

    void setInventory(QStringList);

    void setRoomName(QString);
    void setRoomDesc(QString);
    void setRoomObjs(QString);
    void setRoomPlayers(QString);
    void setRoomExits(QString);
    void setRoomExtra(QString);

    QString getRoomName();
    QString getRoomDesc();
    QString getRoomObjs();
    QString getRoomPlayers();
    QString getRoomExits();
    QString getRoomExtra();

    void setRight(QString);
    void setRightNoun(QString);
    void setLeft(QString);
    void setLeftNoun(QString);

    QString getRight();
    QString getRightNoun();
    QString getLeft();
    QString getLeftNoun();

    void setStanding(bool);
    bool getStanding();

    void setSitting(bool);
    bool getSitting();

    void setKneeling(bool);
    bool getKneeling();

    void setProne(bool);
    bool getProne();

    void setStunned(bool);
    bool getStunned();

    void setBleeding(bool);
    bool getBleeding();

    void setHidden(bool);
    bool getHidden();

    void setInvisible(bool);
    bool getInvisible();

    void setWebbed(bool);
    bool getWebbed();

    void setJoined(bool);
    bool getJoined();

    void setDead(bool);
    bool getDead();

    void setHealth(int);
    int getHealth();

    void setConcentration(int);
    int getConcentration();

    void setSpirit(int);
    int getSpirit();

    void setFatigue(int);
    int getFatigue();

    void setRt(int);
    int getRt();

    QString getExpField(QString name);
    QHash<QString, QString> getExp();
    QMap<QString, int> getExp(QString name);
    void removeExpField(QString);
    QStringList getInventory();
    QStringList getContainer();

    void setActiveSpells(QStringList activeSpells);
    QStringList getActiveSpells();
    void addActiveSpells(QString activeSpell);
    void clearActiveSpells();

    QList<QString> getDirections();
    void setDirections(QList<QString> directions);

private:
    GameDataContainer(QObject *parent = 0);
    GameDataContainer(GameDataContainer const& copy);
    GameDataContainer& operator = (GameDataContainer const& copy);
    static GameDataContainer* m_pInstance;

    TextUtils* textUtils;
    //SharedDataService* sharedDataService;
    QHash<QString, QString> exp;
    QMap<QString, QMap<QString, int> > expMap;
    QStringList container;
    QStringList inventory;

    QRegExp rxNumber;

    QStringList extractExp(QString, bool brief);
    QReadWriteLock lock;

    QString roomName;
    QString roomDesc;
    QString roomObjs;
    QString roomPlayers;
    QString roomExits;
    QString roomExtra;

    QString wieldRightNoun;
    QString wieldRight;
    QString wieldLeftNoun;
    QString wieldLeft;

    int health;
    int concentration;
    int spirit;
    int fatigue;

    bool standing;
    bool sitting;
    bool kneeling;
    bool prone;
    bool stunned;
    bool bleeding;
    bool hidden;
    bool invisible;
    bool webbed;
    bool joined;
    bool dead;

    int rt;

    QStringList activeSpells;

    QList<QString> directions;

signals:
    
public slots:
    
};

#endif // GAMEDATACONTAINER_H
