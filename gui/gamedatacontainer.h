#ifndef GAMEDATACONTAINER_H
#define GAMEDATACONTAINER_H

#include <QObject>
#include <QHash>
#include <QReadLocker>

#include <textutils.h>
#include <shareddataService.h>

class TextUtils;
class SharedDataService;

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
    void setSitting(bool);
    void setKneeling(bool);
    void setProne(bool);
    void setStunned(bool);
    void setBleeding(bool);
    void setHidden(bool);
    void setInvisible(bool);
    void setWebbed(bool);
    void setJoined(bool);
    void setDead(bool);

    void setHealth(int);
    void setConcentration(int);
    void setSpirit(int);
    void setFatigue(int);

    void setRt(int rt);

    QString getExpField(QString name);
    QHash<QString, QString> getExp();
    void removeExpField(QString);
    QStringList getInventory();
    QStringList getContainer();

    void setActiveSpells(QStringList activeSpells);
    QStringList getActiveSpells();
    void addActiveSpells(QString activeSpell);
    void clearActiveSpells();

private:
    GameDataContainer(QObject *parent = 0);
    GameDataContainer(GameDataContainer const& copy);
    GameDataContainer& operator = (GameDataContainer const& copy);
    static GameDataContainer* m_pInstance;

    TextUtils* textUtils;
    SharedDataService* sharedDataService;
    QHash<QString, QString> exp;
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

    QStringList activeSpells;

signals:
    
public slots:
    
};

#endif // GAMEDATACONTAINER_H
