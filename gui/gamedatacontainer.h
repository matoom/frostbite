#ifndef GAMEDATACONTAINER_H
#define GAMEDATACONTAINER_H

#include <QObject>
#include <QHash>
#include <QReadLocker>
#include <QTime>
#include <QRegularExpression>

class TextUtils;

class GameDataContainer : public QObject {
    Q_OBJECT

public:
    static GameDataContainer* Instance();

    void setCharName(QString);
    QString getCharName();

    bool isExpGained(QString name);

    void setExpField(bool brief, QString name, QString exp);
    void setExpField(QString, QString);
    void setExpFieldBrief(QString, QString);

    void setContainer(QStringList);

    void setInventory(QStringList);

    void setRoomName(QString);
    void setRoomDesc(QString);
    void setRoomObjs(QString);    
    void setRoomObjsData(QString);
    void setRoomPlayers(QString);
    void setRoomExits(QString);
    void setRoomExtra(QString);
    void setRoomMonstersBold(QString);

    QString getRoomName();
    QString getRoomDesc();
    QString getRoomObjs();
    QString getRoomObjsData();
    QString getRoomPlayers();
    QString getRoomExits();
    QString getRoomExtra();
    QStringList getRoomMonstersBold();

    void setCompassDirections(QList<QString>);
    QList<QString> getCompassDirections();

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

    void setMana(int);
    int getMana();

    void setFatigue(int);
    int getFatigue();

    void setRt(int);
    int getRt();

    void setCt(int ct);
    int getCt();

    QString getExpField(QString name);
    QHash<QString, QString> getExp();
    QMap<QString, int> getExp(QString name);
    void removeExpField(QString);
    void clearExp();
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

    //SharedDataService* sharedDataService;
    QHash<QString, QString> exp;
    QMap<QString, QMap<QString, int> > expMap;
    QMap<QString, qint64> expGain;
    QStringList container;
    QStringList inventory;

    QStringList extractExp(QString, bool brief);
    QReadWriteLock lock;

    QString charName;

    QString roomName;
    QString roomDesc;
    QString roomObjs;
    QString roomObjsData;
    QString roomPlayers;
    QString roomExits;
    QString roomExtra;    
    QStringList roomMonstersBold;

    QList<QString> compassDirections;

    QString wieldRightNoun;
    QString wieldRight;
    QString wieldLeftNoun;
    QString wieldLeft;

    int health;
    int concentration;
    int spirit;
    int mana;
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
    int ct;

    QStringList activeSpells;

    QList<QString> directions;

signals:
    
public slots:
    
};

#endif // GAMEDATACONTAINER_H
