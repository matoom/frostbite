#ifndef GAMEDATACONTAINER_H
#define GAMEDATACONTAINER_H

#include <QObject>
#include <QHash>

#include <roommodel.h>
#include <wieldmodel.h>
#include <dataconverterservice.h>
#include <dataservice.h>

class DataConverterService;
class DataService;

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

    void setRight(QString);
    void setRightNoun(QString);
    void setLeft(QString);
    void setLeftNoun(QString);

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

    QString getExpField(QString name);
    QHash<QString, QString> getExp();
    void removeExpField(QString);
    RoomModel* getRoom();
    WieldModel* getWield();
    QStringList getInventory();
    QStringList getContainer();

private:
    GameDataContainer(QObject *parent = 0);
    GameDataContainer(GameDataContainer const& copy);
    GameDataContainer& operator = (GameDataContainer const& copy);
    static GameDataContainer* m_pInstance;

    DataConverterService* converter;
    DataService* dataService;
    QHash<QString, QString> exp;
    RoomModel* room;
    WieldModel* wield;
    QStringList container;
    QStringList inventory;

    QStringList extractExp(QString, bool brief);

signals:
    
public slots:
    
};

#endif // GAMEDATACONTAINER_H
