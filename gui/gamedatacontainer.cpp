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
    QWriteLocker locker(&lock);
    /* setting exp for exp window */
    this->exp.insert(name, exp);
    /* extracting exp values for scripting */
    QStringList expList = this->extractExp(exp, false);
    dataService->addExpField(name.toLower().toLocal8Bit().data(),
        expList.at(0).toLocal8Bit().data(), expList.at(1).toLocal8Bit().data());
}

void GameDataContainer::setExpFieldBrief(QString name, QString exp) {
    QWriteLocker locker(&lock);
    /* setting exp brief for exp window */
    this->exp.insert(name, exp);

    /* extracting exp brief values for scripting */
    QStringList expList = this->extractExp(exp, true);
    dataService->addExpField(name.toLower().toLocal8Bit().data(),
        expList.at(0).toLocal8Bit().data(), expList.at(1).toLocal8Bit().data());
}

void GameDataContainer::setContainer(QStringList container) {
    QWriteLocker locker(&lock);
    container.removeFirst();
    dataService->setContainer(container.join(", ").toLocal8Bit().data());
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    QWriteLocker locker(&lock);
    inventory.removeFirst();
    dataService->setInventory(inventory.join(", ").toLocal8Bit().data());
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
    QWriteLocker locker(&lock);
    dataService->removeExpField(name.toLower().toLocal8Bit().data());
    exp.remove(name);
}

QHash<QString, QString> GameDataContainer::getExp() {
    QReadLocker locker(&lock);
    return exp;
}

QStringList GameDataContainer::getInventory() {
    return this->inventory;
}

QStringList GameDataContainer::getContainer() {
    return this->container;
}

void GameDataContainer::setRoomName(QString name) {
    QWriteLocker locker(&lock);
    this->roomName = name;
    dataService->setRoomTitle(name.toLocal8Bit().data());
}

QString GameDataContainer::getRoomName() {
    QReadLocker locker(&lock);
    return this->roomName;
}

void GameDataContainer::setRoomDesc(QString desc) {
    QWriteLocker locker(&lock);
    this->roomDesc = desc;
    dataService->setRoomDescription(desc.toLocal8Bit().data());
}

QString GameDataContainer::getRoomDesc() {
    QReadLocker locker(&lock);
    return this->roomDesc;
}

void GameDataContainer::setRoomObjs(QString objs) {
    QWriteLocker locker(&lock);
    this->roomObjs = objs;
    dataService->setRoomObjects(objs.toLocal8Bit().data());
}

QString GameDataContainer::getRoomObjs() {
    QReadLocker locker(&lock);
    return this->roomObjs;
}

void GameDataContainer::setRoomPlayers(QString players) {
    QWriteLocker locker(&lock);
    this->roomPlayers = players;
    dataService->setRoomPlayers(players.toLocal8Bit().data());
}

QString GameDataContainer::getRoomPlayers() {
    QReadLocker locker(&lock);
    return this->roomPlayers;
}

void GameDataContainer::setRoomExits(QString exits) {
    QWriteLocker locker(&lock);
    this->roomExits = exits;
    dataService->setRoomExits(exits.toLocal8Bit().data());
}

QString GameDataContainer::getRoomExits() {
    QReadLocker locker(&lock);
    return this->roomExits;
}

void GameDataContainer::setRoomExtra(QString extra) {
    QWriteLocker locker(&lock);
    this->roomExtra = extra;
}

QString GameDataContainer::getRoomExtra() {
    QReadLocker locker(&lock);
    return this->roomExtra;
}

void GameDataContainer::setRight(QString right) {
    QWriteLocker locker(&lock);
    this->wieldRight = right;
    dataService->setWieldRight(right.toLocal8Bit().data());
}

QString GameDataContainer::getRight() {
    QReadLocker locker(&lock);
    return this->wieldRight;
}

void GameDataContainer::setRightNoun(QString rightNoun) {
    QWriteLocker locker(&lock);
    this->wieldRightNoun = rightNoun;
    dataService->setWieldRightNoun(rightNoun.toLocal8Bit().data());
}

QString GameDataContainer::getRightNoun() {
    QReadLocker locker(&lock);
    return this->wieldRightNoun;
}

void GameDataContainer::setLeft(QString left) {
    QWriteLocker locker(&lock);
    this->wieldLeft = left;
    dataService->setWieldLeft(left.toLocal8Bit().data());
}

QString GameDataContainer::getLeft() {
    QReadLocker locker(&lock);
    return this->wieldLeft;
}

void GameDataContainer::setLeftNoun(QString leftNoun) {
    QWriteLocker locker(&lock);
    this->wieldLeftNoun = leftNoun;
    dataService->setWieldLeftNoun(leftNoun.toLocal8Bit().data());
}

QString GameDataContainer::getLeftNoun() {
    QReadLocker locker(&lock);
    return this->wieldLeftNoun;
}

void GameDataContainer::setStanding(bool standing) {
    //QWriteLocker locker(&lock);
    dataService->setStanding(standing);
}

void GameDataContainer::setSitting(bool sitting) {
    //QWriteLocker locker(&lock);
    dataService->setSitting(sitting);
}

void GameDataContainer::setKneeling(bool kneeling) {
    //QWriteLocker locker(&lock);
    dataService->setKneeling(kneeling);
}

void GameDataContainer::setProne(bool prone) {
    //QWriteLocker locker(&lock);
    dataService->setProne(prone);
}

void GameDataContainer::setStunned(bool stunned) {
    //QWriteLocker locker(&lock);
    dataService->setStunned(stunned);
}

void GameDataContainer::setBleeding(bool bleeding) {
    //QWriteLocker locker(&lock);
    dataService->setBleeding(bleeding);
}

void GameDataContainer::setHidden(bool hidden) {
    //QWriteLocker locker(&lock);
    dataService->setHidden(hidden);
}

void GameDataContainer::setInvisible(bool invisible) {
    //QWriteLocker locker(&lock);
    dataService->setInvisible(invisible);
}

void GameDataContainer::setWebbed(bool webbed) {
    //QWriteLocker locker(&lock);
    dataService->setWebbed(webbed);
}

void GameDataContainer::setJoined(bool joined) {
    //QWriteLocker locker(&lock);
    dataService->setJoined(joined);
}

void GameDataContainer::setDead(bool dead) {
    //QWriteLocker locker(&lock);
    dataService->setDead(dead);
}

void GameDataContainer::setHealth(int health) {
    //QWriteLocker locker(&lock);
    dataService->setHealth(health);
}

void GameDataContainer::setConcentration(int concentration) {
    //QWriteLocker locker(&lock);
    dataService->setConcentration(concentration);
}

void GameDataContainer::setSpirit(int spirit) {
    //QWriteLocker locker(&lock);
    dataService->setSpirit(spirit);
}

void GameDataContainer::setFatigue(int fatigue) {
    //QWriteLocker locker(&lock);
    dataService->setFatigue(fatigue);
}

void GameDataContainer::setRt(int rt) {
    //QWriteLocker locker(&lock);
    if(rt < 0) {
        rt = 0;
    }
    dataService->setRt(rt);
}
