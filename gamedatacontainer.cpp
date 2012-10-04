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

void GameDataContainer::setExpField(ExpModel* expModel) {
    exp.insert(expModel->getName().toLower(), expModel);
}

void GameDataContainer::removeExpField(QString name) {
    /*TODO FREE POINTERS??*/
    //http://www.qtcentre.org/archive/index.php/t-17500.html
    exp.remove(name);
}

ExpModel* GameDataContainer::getExpField(QString name) {
    return exp.value(name);
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
