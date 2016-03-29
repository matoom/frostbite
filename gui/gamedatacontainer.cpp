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
    //sharedDataService = SharedDataService::Instance();
    rxNumber.setPattern("(\\d+)");

    health = 0;
    concentration = 0;
    spirit = 0;
    fatigue = 0;

    standing = false;
    sitting = false;
    kneeling = false;
    prone = false;
    stunned = false;
    bleeding = false;
    hidden = false;
    invisible = false;
    webbed = false;
    joined = false;
    dead = false;

    rt = 0;
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
    QStringList expList = this->extractExp(exp, false);

    /* extracting exp values for scripting */
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->addExpField(name.toLower().toLocal8Bit().data(),
                                 expList.at(0).toInt(), expList.at(1).toInt());
    }*/

    QMap<QString, int> expValueMap;
    expValueMap["rank"] = expList.at(0).toInt();
    expValueMap["state"] = expList.at(1).toInt();

    QWriteLocker locker(&lock);
    this->expMap.insert(name.toLower(), expValueMap);

    /* setting exp for exp window */
    this->exp.insert(name, textUtils->addNumericStateToExp(exp));        
}

void GameDataContainer::setExpFieldBrief(QString name, QString exp) {
    QStringList expList = this->extractExp(exp, true);

    /* extracting exp brief values for scripting */
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->addExpField(name.toLower().toLocal8Bit().data(),
                                 expList.at(0).toInt(), expList.at(1).toInt());
    }*/

    QMap<QString, int> expValueMap;
    expValueMap["rank"] = expList.at(0).toInt();
    expValueMap["state"] = expList.at(1).toInt();

    QWriteLocker locker(&lock);
    this->expMap.insert(name.toLower(), expValueMap);

    /* setting exp brief for exp window */
    this->exp.insert(name, exp);
}

void GameDataContainer::setContainer(QStringList container) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setContainer(container.join(", ").toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    container.removeFirst();
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setInventory(inventory.join(", ").toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    inventory.removeFirst();
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
    QWriteLocker locker(&lock);
    exp.remove(name);
    expMap.remove(name.toLower());


    /*QMap<QString, int> expValueMap;
    expValueMap["rank"] = 0;
    expValueMap["state"] = 0;*/

    //expMap.insert(name, expValueMap);

    /*if(sharedDataService->isLoaded()) {
        sharedDataService->removeExpField(name.toLower().toLocal8Bit().data());
    }*/
}

QHash<QString, QString> GameDataContainer::getExp() {
    QReadLocker locker(&lock);
    return exp;
}

QMap<QString, int> GameDataContainer::getExp(QString name) {
    lock.lockForRead();
    QMap<QString, QMap<QString, int> > exp = this->expMap;
    lock.unlock();

    QMap<QString, int> expValueMap;

    QMap<QString, QMap<QString, int> >::iterator find = exp.find(name);
    if(find != exp.end()) {
        expValueMap = find.value();
    } else {
        QMapIterator<QString, QMap<QString, int> > nearestValue(exp);
        while (nearestValue.hasNext()) {
            nearestValue.next();
            size_t nameSize = strlen(name.toLocal8Bit().data());
            const char* c = nearestValue.key().toLocal8Bit().data();
            for(unsigned int j = 0; j < nameSize; j++) {
                if(c[j] == 0 || c[j] != name[j]) {
                    break;
                } else {
                    if(j == nameSize - 1) {
                        expValueMap = nearestValue.value();
                    }
                }
            }
        }
    }
    return expValueMap;
}

QStringList GameDataContainer::getInventory() {
    return this->inventory;
}

QStringList GameDataContainer::getContainer() {
    return this->container;
}

void GameDataContainer::setRoomName(QString name) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomTitle(name.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->roomName = name;
}

QString GameDataContainer::getRoomName() {
    QReadLocker locker(&lock);
    return this->roomName;
}

void GameDataContainer::setRoomDesc(QString desc) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomDescription(desc.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->roomDesc = desc;
}

QString GameDataContainer::getRoomDesc() {
    QReadLocker locker(&lock);
    return this->roomDesc;
}

void GameDataContainer::setRoomObjs(QString objs) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomObjects(objs.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->roomObjs = objs;
}

QString GameDataContainer::getRoomObjs() {
    QReadLocker locker(&lock);
    return this->roomObjs;
}

void GameDataContainer::setRoomPlayers(QString players) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomPlayers(players.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->roomPlayers = players;
}

QString GameDataContainer::getRoomPlayers() {
    QReadLocker locker(&lock);
    return this->roomPlayers;
}

void GameDataContainer::setRoomExits(QString exits) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setRoomExits(exits.toLocal8Bit().data());
    }*/
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
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldRight(right.toLocal8Bit().data());
    }*/
}

QString GameDataContainer::getRight() {
    QReadLocker locker(&lock);
    return this->wieldRight;
}

void GameDataContainer::setRightNoun(QString rightNoun) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldRightNoun(rightNoun.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->wieldRightNoun = rightNoun;
}

QString GameDataContainer::getRightNoun() {
    QReadLocker locker(&lock);
    return this->wieldRightNoun;
}

void GameDataContainer::setLeft(QString left) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldLeft(left.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->wieldLeft = left;
}

QString GameDataContainer::getLeft() {
    QReadLocker locker(&lock);
    return this->wieldLeft;
}

void GameDataContainer::setLeftNoun(QString leftNoun) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setWieldLeftNoun(leftNoun.toLocal8Bit().data());
    }*/
    QWriteLocker locker(&lock);
    this->wieldLeftNoun = leftNoun;
}

QString GameDataContainer::getLeftNoun() {
    QReadLocker locker(&lock);
    return this->wieldLeftNoun;
}

void GameDataContainer::setStanding(bool standing) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setStanding(standing);
    }*/
    QWriteLocker locker(&lock);
    this->standing = standing;
}

bool GameDataContainer::getStanding() {
    QReadLocker locker(&lock);
    return this->standing;
}

void GameDataContainer::setSitting(bool sitting) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setSitting(sitting);
    }*/
    QWriteLocker locker(&lock);
    this->sitting = sitting;
}

bool GameDataContainer::getSitting() {
    QReadLocker locker(&lock);
    return this->sitting;
}

void GameDataContainer::setKneeling(bool kneeling) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setKneeling(kneeling);
    }*/
    QWriteLocker locker(&lock);
    this->kneeling = kneeling;
}

bool GameDataContainer::getKneeling() {
    QReadLocker locker(&lock);
    return this->kneeling;
}

void GameDataContainer::setProne(bool prone) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setProne(prone);
    }*/
    QWriteLocker locker(&lock);
    this->prone = prone;
}

bool GameDataContainer::getProne() {
    QReadLocker locker(&lock);
    return this->prone;
}

void GameDataContainer::setStunned(bool stunned) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setStunned(stunned);
    }*/
    QWriteLocker locker(&lock);
    this->stunned = stunned;
}

bool GameDataContainer::getStunned() {
    QReadLocker locker(&lock);
    return this->stunned;
}

void GameDataContainer::setBleeding(bool bleeding) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setBleeding(bleeding);
    }*/
    QWriteLocker locker(&lock);
    this->bleeding = bleeding;
}

bool GameDataContainer::getBleeding() {
    QReadLocker locker(&lock);
    return this->bleeding;
}

void GameDataContainer::setHidden(bool hidden) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setHidden(hidden);
    }*/
    QWriteLocker locker(&lock);
    this->hidden = hidden;
}

bool GameDataContainer::getHidden() {
    QReadLocker locker(&lock);
    return this->hidden;
}

void GameDataContainer::setInvisible(bool invisible) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setInvisible(invisible);
    }*/
    QWriteLocker locker(&lock);
    this->invisible = invisible;
}

bool GameDataContainer::getInvisible() {
    QReadLocker locker(&lock);
    return this->invisible;
}

void GameDataContainer::setWebbed(bool webbed) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setWebbed(webbed);
    }*/
    QWriteLocker locker(&lock);
    this->webbed = webbed;
}

bool GameDataContainer::getWebbed() {
    QReadLocker locker(&lock);
    return this->webbed;
}

void GameDataContainer::setJoined(bool joined) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setJoined(joined);
    }*/
    QWriteLocker locker(&lock);
    this->joined = joined;
}

bool GameDataContainer::getJoined() {
    QReadLocker locker(&lock);
    return this->joined;
}

void GameDataContainer::setDead(bool dead) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setDead(dead);
    }*/
    QWriteLocker locker(&lock);
    this->dead = dead;
}

bool GameDataContainer::getDead() {
    QReadLocker locker(&lock);
    return this->dead;
}

void GameDataContainer::setHealth(int health) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setHealth(health);
    }*/
    QWriteLocker locker(&lock);
    this->health = health;
}

int GameDataContainer::getHealth() {
    QReadLocker locker(&lock);
    return this->health;
}

void GameDataContainer::setConcentration(int concentration) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setConcentration(concentration);
    }*/
    QWriteLocker locker(&lock);
    this->concentration = concentration;
}

int GameDataContainer::getConcentration() {
    QReadLocker locker(&lock);
    return this->concentration;
}

void GameDataContainer::setSpirit(int spirit) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setSpirit(spirit);
    }*/
    QWriteLocker locker(&lock);
    this->spirit = spirit;
}

int GameDataContainer::getSpirit() {
    QReadLocker locker(&lock);
    return this->spirit;
}

void GameDataContainer::setFatigue(int fatigue) {
    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setFatigue(fatigue);
    }*/
    QWriteLocker locker(&lock);
    this->fatigue = fatigue;
}

int GameDataContainer::getFatigue() {
    QReadLocker locker(&lock);
    return this->fatigue;
}

void GameDataContainer::setRt(int rt) {
    if(rt < 0) rt = 0;

    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setRt(rt);
    }*/

    QWriteLocker locker(&lock);
    this->rt = rt;
}

int GameDataContainer::getRt() {
    QReadLocker locker(&lock);
    return this->rt;
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

    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setActiveSpells(activeSpells.join("\n").toLocal8Bit().data());
    }*/
}

void GameDataContainer::clearActiveSpells() {
    QWriteLocker locker(&lock);
    this->activeSpells.clear();

    /*if(sharedDataService->isLoaded()) {
        sharedDataService->setActiveSpells("");
    }*/
}


