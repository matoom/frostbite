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
    rxNumber.setPattern("(\\d+)");
}

QStringList GameDataContainer::extractExp(QString exp, bool brief) {
    QStringList result;

    rxNumber.indexIn(exp, 0);
    result << rxNumber.cap(1);

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
    this->exp.insert(name, converter->addNumericStateToExp(exp));

    /* extracting exp values for scripting */
    if(dataService->isLoaded()) {
        QStringList expList = this->extractExp(exp, false);
        dataService->addExpField(name.toLower().toLocal8Bit().data(),
            expList.at(0).toLocal8Bit().data(), expList.at(1).toLocal8Bit().data());
    }
}

void GameDataContainer::setExpFieldBrief(QString name, QString exp) {
    QWriteLocker locker(&lock);
    /* setting exp brief for exp window */
    this->exp.insert(name, exp);

    /* extracting exp brief values for scripting */
    if(dataService->isLoaded()) {
        QStringList expList = this->extractExp(exp, true);
        dataService->addExpField(name.toLower().toLocal8Bit().data(),
            expList.at(0).toLocal8Bit().data(), expList.at(1).toLocal8Bit().data());
    }
}

void GameDataContainer::setContainer(QStringList container) {
    if(dataService->isLoaded()) {
        dataService->setContainer(container.join(", ").toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    container.removeFirst();
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    if(dataService->isLoaded()) {
        dataService->setInventory(inventory.join(", ").toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    inventory.removeFirst();
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
    if(dataService->isLoaded()) {
        dataService->removeExpField(name.toLower().toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
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
    if(dataService->isLoaded()) {
        dataService->setRoomTitle(name.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomName = name;
}

QString GameDataContainer::getRoomName() {
    QReadLocker locker(&lock);
    return this->roomName;
}

void GameDataContainer::setRoomDesc(QString desc) {
    if(dataService->isLoaded()) {
        dataService->setRoomDescription(desc.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomDesc = desc;
}

QString GameDataContainer::getRoomDesc() {
    QReadLocker locker(&lock);
    return this->roomDesc;
}

void GameDataContainer::setRoomObjs(QString objs) {
    if(dataService->isLoaded()) {
        dataService->setRoomObjects(objs.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomObjs = objs;
}

QString GameDataContainer::getRoomObjs() {
    QReadLocker locker(&lock);
    return this->roomObjs;
}

void GameDataContainer::setRoomPlayers(QString players) {
    if(dataService->isLoaded()) {
        dataService->setRoomPlayers(players.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomPlayers = players;
}

QString GameDataContainer::getRoomPlayers() {
    QReadLocker locker(&lock);
    return this->roomPlayers;
}

void GameDataContainer::setRoomExits(QString exits) {
    if(dataService->isLoaded()) {
        dataService->setRoomExits(exits.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomExits = exits;
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
    if(dataService->isLoaded()) {
        dataService->setWieldRight(right.toLocal8Bit().data());
    }
}

QString GameDataContainer::getRight() {
    QReadLocker locker(&lock);
    return this->wieldRight;
}

void GameDataContainer::setRightNoun(QString rightNoun) {
    if(dataService->isLoaded()) {
        dataService->setWieldRightNoun(rightNoun.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->wieldRightNoun = rightNoun;
}

QString GameDataContainer::getRightNoun() {
    QReadLocker locker(&lock);
    return this->wieldRightNoun;
}

void GameDataContainer::setLeft(QString left) {
    if(dataService->isLoaded()) {
        dataService->setWieldLeft(left.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->wieldLeft = left;
}

QString GameDataContainer::getLeft() {
    QReadLocker locker(&lock);
    return this->wieldLeft;
}

void GameDataContainer::setLeftNoun(QString leftNoun) {
    if(dataService->isLoaded()) {
        dataService->setWieldLeftNoun(leftNoun.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->wieldLeftNoun = leftNoun;
}

QString GameDataContainer::getLeftNoun() {
    QReadLocker locker(&lock);
    return this->wieldLeftNoun;
}

void GameDataContainer::setStanding(bool standing) {
    if(dataService->isLoaded()) {
        dataService->setStanding(standing);
    }
}

void GameDataContainer::setSitting(bool sitting) {
    if(dataService->isLoaded()) {
        dataService->setSitting(sitting);
    }
}

void GameDataContainer::setKneeling(bool kneeling) {
    if(dataService->isLoaded()) {
        dataService->setKneeling(kneeling);
    }
}

void GameDataContainer::setProne(bool prone) {
    if(dataService->isLoaded()) {
        dataService->setProne(prone);
    }
}

void GameDataContainer::setStunned(bool stunned) {
    if(dataService->isLoaded()) {
        dataService->setStunned(stunned);
    }
}

void GameDataContainer::setBleeding(bool bleeding) {
    if(dataService->isLoaded()) {
        dataService->setBleeding(bleeding);
    }
}

void GameDataContainer::setHidden(bool hidden) {
    if(dataService->isLoaded()) {
        dataService->setHidden(hidden);
    }
}

void GameDataContainer::setInvisible(bool invisible) {
    if(dataService->isLoaded()) {
        dataService->setInvisible(invisible);
    }
}

void GameDataContainer::setWebbed(bool webbed) {
    if(dataService->isLoaded()) {
        dataService->setWebbed(webbed);
    }
}

void GameDataContainer::setJoined(bool joined) {
    if(dataService->isLoaded()) {
        dataService->setJoined(joined);
    }
}

void GameDataContainer::setDead(bool dead) {
    if(dataService->isLoaded()) {
        dataService->setDead(dead);
    }
}

void GameDataContainer::setHealth(int health) {
    if(dataService->isLoaded()) {
        dataService->setHealth(health);
    }
}

void GameDataContainer::setConcentration(int concentration) {
    if(dataService->isLoaded()) {
        dataService->setConcentration(concentration);
    }
}

void GameDataContainer::setSpirit(int spirit) {
    if(dataService->isLoaded()) {
        dataService->setSpirit(spirit);
    }
}

void GameDataContainer::setFatigue(int fatigue) {
    if(dataService->isLoaded()) {
        dataService->setFatigue(fatigue);
    }
}

void GameDataContainer::setRt(int rt) {
    if(dataService->isLoaded()) {
        if(rt < 0) {
            rt = 0;
        }
        dataService->setRt(rt);
    }
}
