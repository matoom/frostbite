#ifndef GAMEDATACONTAINER_H
#define GAMEDATACONTAINER_H

#include <QObject>
#include <QHash>
#include <expmodel.h>
#include <roommodel.h>
#include <wieldmodel.h>

class GameDataContainer : public QObject {
    Q_OBJECT

public:
    static GameDataContainer* Instance();

    void setExpField(QString, ExpModel*);
    void setContainer(QStringList);
    void setInventory(QStringList);
    ExpModel* getExpField(QString name);
    QHash<QString, ExpModel*> getExp();
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

    QHash<QString, ExpModel*> exp;
    RoomModel* room;
    WieldModel* wield;
    QStringList container;
    QStringList inventory;

signals:
    
public slots:
    
};

#endif // GAMEDATACONTAINER_H
