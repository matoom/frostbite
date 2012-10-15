#include "gamedatacontainer.h"

GameDataContainer* GameDataContainer::m_pInstance = NULL;

GameDataContainer* GameDataContainer::Instance() {
    if (!m_pInstance) {
        m_pInstance = new GameDataContainer;
    }

    return m_pInstance;
}

GameDataContainer::GameDataContainer(QObject *parent) : QObject(parent) {
    room = new RoomModel();
    wield = new WieldModel();
}

void GameDataContainer::setExpField(QString name, ExpModel* expModel) {
    exp.insert(name.toLower(), expModel);
}

void GameDataContainer::setContainer(QStringList container) {
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
    /*TODO FREE POINTERS??*/
    //http://www.qtcentre.org/archive/index.php/t-17500.html
    exp.remove(name);
}

ExpModel* GameDataContainer::getExpField(QString name) {
    return exp.value(name.toLower());
}

QHash<QString, ExpModel*> GameDataContainer::getExp() {
    return exp;
}

RoomModel* GameDataContainer::getRoom() {
    return this->room;
}

WieldModel* GameDataContainer::getWield() {
    return this->wield;
}

QStringList GameDataContainer::getInventory() {
    return this->inventory;
}

QStringList GameDataContainer::getContainer() {
    return this->container;
}

