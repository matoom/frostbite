#include "gamedatacontainer.h"

GameDataContainer* GameDataContainer::m_pInstance = NULL;

GameDataContainer* GameDataContainer::Instance() {
    if (!m_pInstance) {
        m_pInstance = new GameDataContainer;
    }
    return m_pInstance;
}

GameDataContainer::GameDataContainer(QObject *parent) : QObject(parent) {
    converter = DataConverterService::Instance();
    dataService = DataService::Instance();

    room = new RoomModel();
    wield = new WieldModel();
}

QStringList GameDataContainer::extractExp(QString exp, bool brief) {
    QStringList result;

    QRegExp rx("(\\d+)");
    rx.indexIn(exp, 0);
    result << rx.cap(1);

    int index = exp.indexOf('%') + 1;
    if(brief) {
        result << QString::number(converter->expBriefToNumeric(exp.mid(index).trimmed()));
    } else {
        result << QString::number(converter->expStateToNumeric(exp.mid(index).trimmed()));
    }
    return result;
}

void GameDataContainer::setExpField(QString name, QString exp) {
    this->exp.insert(name, exp);
    QStringList expList = this->extractExp(exp, false);
    dataService->addExpField(name.toLower().toLocal8Bit().data(),
        expList.at(0).toLocal8Bit().data(), expList.at(1).toLocal8Bit().data());
}

void GameDataContainer::setExpFieldBrief(QString name, QString exp) {
    this->exp.insert(name, exp);
    QStringList expList = this->extractExp(exp, true);
    dataService->addExpField(name.toLower().toLocal8Bit().data(),
        expList.at(0).toLocal8Bit().data(), expList.at(1).toLocal8Bit().data());
}

void GameDataContainer::setContainer(QStringList container) {
    container.removeFirst();
    dataService->setContainer(container.join(", ").toLocal8Bit().data());
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    inventory.removeFirst();
    dataService->setInventory(inventory.join(", ").toLocal8Bit().data());
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
    dataService->removeExpField(name.toLower().toLocal8Bit().data());
    exp.remove(name);
}

QHash<QString, QString> GameDataContainer::getExp() {
    return exp;
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

RoomModel* GameDataContainer::getRoom() {
    return this->room;
}

void GameDataContainer::setRoomName(QString name) {
    this->room->setName(name);
    dataService->setRoomTitle(name.toLocal8Bit().data());
}

void GameDataContainer::setRoomDesc(QString desc) {
    this->room->setDesc(desc);
    dataService->setRoomDescription(desc.toLocal8Bit().data());
}

void GameDataContainer::setRoomObjs(QString objs) {
    this->room->setObjs(objs);
    dataService->setRoomObjects(objs.toLocal8Bit().data());
}

void GameDataContainer::setRoomPlayers(QString players) {
    this->room->setPlayers(players);
    dataService->setRoomPlayers(players.toLocal8Bit().data());
}

void GameDataContainer::setRoomExits(QString exits) {
    this->room->setExits(exits);
    dataService->setRoomExits(exits.toLocal8Bit().data());
}

void GameDataContainer::setRoomExtra(QString extra) {
    this->room->setExtra(extra);
}

void GameDataContainer::setRight(QString right) {
    this->wield->setRight(right);
    dataService->setWieldRight(right.toLocal8Bit().data());
}

void GameDataContainer::setRightNoun(QString rightNoun) {
    this->wield->setRightNoun(rightNoun);
    dataService->setWieldRightNoun(rightNoun.toLocal8Bit().data());
}

void GameDataContainer::setLeft(QString left) {
    this->wield->setLeft(left);
    dataService->setWieldLeft(left.toLocal8Bit().data());
}

void GameDataContainer::setLeftNoun(QString leftNoun) {
    this->wield->setLeftNoun(leftNoun);
    dataService->setWieldLeftNoun(leftNoun.toLocal8Bit().data());
}

void GameDataContainer::setStanding(bool standing) {
    dataService->setStanding(standing);
}

void GameDataContainer::setSitting(bool sitting) {
    dataService->setSitting(sitting);
}

void GameDataContainer::setKneeling(bool kneeling) {
    dataService->setKneeling(kneeling);
}

void GameDataContainer::setProne(bool prone) {
    dataService->setProne(prone);
}

void GameDataContainer::setStunned(bool stunned) {
    dataService->setStunned(stunned);
}

void GameDataContainer::setBleeding(bool bleeding) {
    dataService->setBleeding(bleeding);
}

void GameDataContainer::setHidden(bool hidden) {
    dataService->setHidden(hidden);
}

void GameDataContainer::setInvisible(bool invisible) {
    dataService->setInvisible(invisible);
}

void GameDataContainer::setWebbed(bool webbed) {
    dataService->setWebbed(webbed);
}

void GameDataContainer::setJoined(bool joined) {
    dataService->setJoined(joined);
}

void GameDataContainer::setDead(bool dead) {
    dataService->setDead(dead);
}

void GameDataContainer::setHealth(int health) {
    dataService->setHealth(health);
}

void GameDataContainer::setConcentration(int concentration) {
    dataService->setConcentration(concentration);
}

void GameDataContainer::setSpirit(int spirit) {
    dataService->setSpirit(spirit);
}

void GameDataContainer::setFatigue(int fatigue) {
    dataService->setFatigue(fatigue);
}

