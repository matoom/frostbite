#include "gamedatacontainer.h"

GameDataContainer* GameDataContainer::m_pInstance = NULL;

GameDataContainer* GameDataContainer::Instance() {
    if (!m_pInstance) {
        m_pInstance = new GameDataContainer;
    }
    return m_pInstance;
}

GameDataContainer::GameDataContainer(QObject *parent) : QObject(parent) {
    textUtils = TextUtils::Instance();
    sharedDataService = SharedDataService::Instance();
    rxNumber.setPattern("(\\d+)");
}

QStringList GameDataContainer::extractExp(QString exp, bool brief) {
    QStringList result;

    rxNumber.indexIn(exp, 0);
    result << rxNumber.cap(1);

    int index = exp.indexOf('%') + 1;
    if(brief) {
        result << QString::number(textUtils->expBriefToNumeric(exp.mid(index).trimmed()));
    } else {
        result << QString::number(textUtils->expStateToNumeric(exp.mid(index).trimmed()));
    }
    return result;
}

void GameDataContainer::setExpField(QString name, QString exp) {
    //qDebug() << name;

    /* extracting exp values for scripting */
    if(sharedDataService->isLoaded()) {
        QStringList expList = this->extractExp(exp, false);
        sharedDataService->addExpField(name.toLower().toLocal8Bit().data(),
                                 expList.at(0).toInt(), expList.at(1).toInt());
    }

    QWriteLocker locker(&lock);
    /* setting exp for exp window */
    this->exp.insert(name, textUtils->addNumericStateToExp(exp));
}

void GameDataContainer::setExpFieldBrief(QString name, QString exp) {
    //qDebug() << name;

    /* extracting exp brief values for scripting */
    if(sharedDataService->isLoaded()) {
        QStringList expList = this->extractExp(exp, true);
        sharedDataService->addExpField(name.toLower().toLocal8Bit().data(),
                                 expList.at(0).toInt(), expList.at(1).toInt());
    }

    QWriteLocker locker(&lock);
    /* setting exp brief for exp window */
    this->exp.insert(name, exp);
}

void GameDataContainer::setContainer(QStringList container) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setContainer(container.join(", ").toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    container.removeFirst();
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setInventory(inventory.join(", ").toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    inventory.removeFirst();
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
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
    if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomTitle(name.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomName = name;
}

QString GameDataContainer::getRoomName() {
    QReadLocker locker(&lock);
    return this->roomName;
}

void GameDataContainer::setRoomDesc(QString desc) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomDescription(desc.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomDesc = desc;
}

QString GameDataContainer::getRoomDesc() {
    QReadLocker locker(&lock);
    return this->roomDesc;
}

void GameDataContainer::setRoomObjs(QString objs) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomObjects(objs.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomObjs = objs;
}

QString GameDataContainer::getRoomObjs() {
    QReadLocker locker(&lock);
    return this->roomObjs;
}

void GameDataContainer::setRoomPlayers(QString players) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomPlayers(players.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->roomPlayers = players;
}

QString GameDataContainer::getRoomPlayers() {
    QReadLocker locker(&lock);
    return this->roomPlayers;
}

void GameDataContainer::setRoomExits(QString exits) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomExits(exits.toLocal8Bit().data());
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
    if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldRight(right.toLocal8Bit().data());
    }
}

QString GameDataContainer::getRight() {
    QReadLocker locker(&lock);
    return this->wieldRight;
}

void GameDataContainer::setRightNoun(QString rightNoun) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldRightNoun(rightNoun.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->wieldRightNoun = rightNoun;
}

QString GameDataContainer::getRightNoun() {
    QReadLocker locker(&lock);
    return this->wieldRightNoun;
}

void GameDataContainer::setLeft(QString left) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldLeft(left.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->wieldLeft = left;
}

QString GameDataContainer::getLeft() {
    QReadLocker locker(&lock);
    return this->wieldLeft;
}

void GameDataContainer::setLeftNoun(QString leftNoun) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldLeftNoun(leftNoun.toLocal8Bit().data());
    }
    QWriteLocker locker(&lock);
    this->wieldLeftNoun = leftNoun;
}

QString GameDataContainer::getLeftNoun() {
    QReadLocker locker(&lock);
    return this->wieldLeftNoun;
}

void GameDataContainer::setStanding(bool standing) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setStanding(standing);
    }
}

void GameDataContainer::setSitting(bool sitting) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setSitting(sitting);
    }
}

void GameDataContainer::setKneeling(bool kneeling) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setKneeling(kneeling);
    }
}

void GameDataContainer::setProne(bool prone) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setProne(prone);
    }
}

void GameDataContainer::setStunned(bool stunned) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setStunned(stunned);
    }
}

void GameDataContainer::setBleeding(bool bleeding) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setBleeding(bleeding);
    }
}

void GameDataContainer::setHidden(bool hidden) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setHidden(hidden);
    }
}

void GameDataContainer::setInvisible(bool invisible) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setInvisible(invisible);
    }
}

void GameDataContainer::setWebbed(bool webbed) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setWebbed(webbed);
    }
}

void GameDataContainer::setJoined(bool joined) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setJoined(joined);
    }
}

void GameDataContainer::setDead(bool dead) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setDead(dead);
    }
}

void GameDataContainer::setHealth(int health) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setHealth(health);
    }
}

void GameDataContainer::setConcentration(int concentration) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setConcentration(concentration);
    }
}

void GameDataContainer::setSpirit(int spirit) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setSpirit(spirit);
    }
}

void GameDataContainer::setFatigue(int fatigue) {
    if(sharedDataService->isLoaded()) {
        sharedDataService->setFatigue(fatigue);
    }
}

void GameDataContainer::setRt(int rt) {
    if(sharedDataService->isLoaded()) {
        if(rt < 0) {
            rt = 0;
        }
        sharedDataService->setRt(rt);
    }
}

void GameDataContainer::setActiveSpells(QStringList activeSpells) {
    QWriteLocker locker(&lock);
    this->activeSpells = activeSpells;
}

QStringList GameDataContainer::getActiveSpells() {
    QReadLocker locker(&lock);
    return this->activeSpells;
}

void GameDataContainer::addActiveSpells(QString activeSpell) {
    QWriteLocker locker(&lock);
    this->activeSpells << activeSpell;

    if(sharedDataService->isLoaded()) {
        sharedDataService->setActiveSpells(activeSpells.join("\n").toLocal8Bit().data());
    }
}

void GameDataContainer::clearActiveSpells() {
    QWriteLocker locker(&lock);
    this->activeSpells.clear();

    if(sharedDataService->isLoaded()) {
        sharedDataService->setActiveSpells("");
    }
}


