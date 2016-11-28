#include "gamedatacontainer.h"

GameDataContainer* GameDataContainer::m_pInstance = NULL;

GameDataContainer* GameDataContainer::Instance() {
    if (!m_pInstance) {
        m_pInstance = new GameDataContainer;
    }
    return m_pInstance;
}

GameDataContainer::GameDataContainer(QObject *parent) : QObject(parent) {
    health = 0;
    concentration = 0;
    spirit = 0;
    fatigue = 0;
    mana = 0;

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
    ct = 0;
}

QStringList GameDataContainer::extractExp(QString exp, bool brief) {        
    QStringList result;

    QRegExp rxNumber("(\\d+)");
    rxNumber.indexIn(exp, 0);
    result << rxNumber.cap(1);

    int index = exp.indexOf('%') + 1;
    if(brief) {
        result << QString::number(TextUtils::expBriefToNumeric(exp.mid(index).trimmed()));
    } else {
        result << QString::number(TextUtils::expStateToNumeric(exp.mid(index).trimmed()));
    }
    return result;
}

bool GameDataContainer::isExpGained(QString name) {
    QReadLocker locker(&lock);
    qint64 gain = this->expGain.value(name);
    int state = this->expMap.value(name.toLower()).value("state");
    return (gain != 0 && (QDateTime::currentMSecsSinceEpoch() - gain) < 120000) || state == 34;
}

void GameDataContainer::setExpField(bool brief, QString name, QString exp) {
    int state = this->expMap.value(name.toLower()).value("state");

    if(brief) {        
        this->setExpFieldBrief(name, exp);
    } else {
        this->setExpField(name, exp);
    }

    if(this->expMap.value(name.toLower()).value("state") > state) {
        QWriteLocker locker(&lock);
        expGain.insert(name, QDateTime::currentMSecsSinceEpoch());
    }
}

void GameDataContainer::setExpField(QString name, QString exp) {
    QStringList expList = this->extractExp(exp, false);

    /* extracting exp values for scripting */
    QMap<QString, int> expValueMap;
    expValueMap["rank"] = expList.at(0).toInt();
    expValueMap["state"] = expList.at(1).toInt();

    QWriteLocker locker(&lock);
    this->expMap.insert(name.toLower(), expValueMap);

    /* setting exp for exp window */
    this->exp.insert(name, TextUtils::addNumericStateToExp(exp));
}

void GameDataContainer::setExpFieldBrief(QString name, QString exp) {
    QStringList expList = this->extractExp(exp, true);

    /* extracting exp brief values for scripting */
    QMap<QString, int> expValueMap;
    expValueMap["rank"] = expList.at(0).toInt();
    expValueMap["state"] = expList.at(1).toInt();

    QWriteLocker locker(&lock);
    this->expMap.insert(name.toLower(), expValueMap);

    /* setting exp brief for exp window */
    this->exp.insert(name, exp);
}

void GameDataContainer::setContainer(QStringList container) {
    QWriteLocker locker(&lock);
    container.removeFirst();
    this->container = container;
}

void GameDataContainer::setInventory(QStringList inventory) {
    QWriteLocker locker(&lock);
    if(!inventory.isEmpty()) inventory.removeFirst();
    this->inventory = inventory;
}

void GameDataContainer::removeExpField(QString name) {
    QWriteLocker locker(&lock);
    exp.remove(name);
    expMap.remove(name.toLower());
}

void GameDataContainer::clearExp() {
    QWriteLocker locker(&lock);
    exp.clear();
    expMap.clear();
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
    QWriteLocker locker(&lock);
    this->roomName = name;
}

QString GameDataContainer::getRoomName() {
    QReadLocker locker(&lock);
    return this->roomName;
}

void GameDataContainer::setRoomDesc(QString desc) {
    QWriteLocker locker(&lock);
    this->roomDesc = desc;
}

QString GameDataContainer::getRoomDesc() {
    QReadLocker locker(&lock);
    return this->roomDesc;
}

void GameDataContainer::setRoomObjsData(QString objs) {
    QWriteLocker locker(&lock);
    this->roomObjsData = objs;
}

QString GameDataContainer::getRoomObjsData() {
    QReadLocker locker(&lock);
    return this->roomObjsData;
}

void GameDataContainer::setRoomObjs(QString objs) {
    QWriteLocker locker(&lock);
    this->roomObjs = objs;
}

QString GameDataContainer::getRoomObjs() {
    QReadLocker locker(&lock);
    return this->roomObjs;
}

void GameDataContainer::setRoomPlayers(QString players) {
    QWriteLocker locker(&lock);
    this->roomPlayers = players;
}

QString GameDataContainer::getRoomPlayers() {
    QReadLocker locker(&lock);
    return this->roomPlayers;
}

void GameDataContainer::setRoomExits(QString exits) {
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

void GameDataContainer::setCompassDirections(QList<QString> compassDirections) {
    QWriteLocker locker(&lock);
    this->compassDirections = compassDirections;
}

QList<QString> GameDataContainer::getCompassDirections() {
    QReadLocker locker(&lock);
    return this->compassDirections;
}

void GameDataContainer::setRight(QString right) {
    QWriteLocker locker(&lock);
    this->wieldRight = right;
}

QString GameDataContainer::getRight() {
    QReadLocker locker(&lock);
    return this->wieldRight;
}

void GameDataContainer::setRightNoun(QString rightNoun) {
    QWriteLocker locker(&lock);
    this->wieldRightNoun = rightNoun;
}

QString GameDataContainer::getRightNoun() {
    QReadLocker locker(&lock);
    return this->wieldRightNoun;
}

void GameDataContainer::setLeft(QString left) {
    QWriteLocker locker(&lock);
    this->wieldLeft = left;
}

QString GameDataContainer::getLeft() {
    QReadLocker locker(&lock);
    return this->wieldLeft;
}

void GameDataContainer::setLeftNoun(QString leftNoun) {
    QWriteLocker locker(&lock);
    this->wieldLeftNoun = leftNoun;
}

QString GameDataContainer::getLeftNoun() {
    QReadLocker locker(&lock);
    return this->wieldLeftNoun;
}

void GameDataContainer::setStanding(bool standing) {
    QWriteLocker locker(&lock);
    this->standing = standing;
}

bool GameDataContainer::getStanding() {
    QReadLocker locker(&lock);
    return this->standing;
}

void GameDataContainer::setSitting(bool sitting) {
    QWriteLocker locker(&lock);
    this->sitting = sitting;
}

bool GameDataContainer::getSitting() {
    QReadLocker locker(&lock);
    return this->sitting;
}

void GameDataContainer::setKneeling(bool kneeling) {
    QWriteLocker locker(&lock);
    this->kneeling = kneeling;
}

bool GameDataContainer::getKneeling() {
    QReadLocker locker(&lock);
    return this->kneeling;
}

void GameDataContainer::setProne(bool prone) {
    QWriteLocker locker(&lock);
    this->prone = prone;
}

bool GameDataContainer::getProne() {
    QReadLocker locker(&lock);
    return this->prone;
}

void GameDataContainer::setStunned(bool stunned) {
    QWriteLocker locker(&lock);
    this->stunned = stunned;
}

bool GameDataContainer::getStunned() {
    QReadLocker locker(&lock);
    return this->stunned;
}

void GameDataContainer::setBleeding(bool bleeding) {
    QWriteLocker locker(&lock);
    this->bleeding = bleeding;
}

bool GameDataContainer::getBleeding() {
    QReadLocker locker(&lock);
    return this->bleeding;
}

void GameDataContainer::setHidden(bool hidden) {
    QWriteLocker locker(&lock);
    this->hidden = hidden;
}

bool GameDataContainer::getHidden() {
    QReadLocker locker(&lock);
    return this->hidden;
}

void GameDataContainer::setInvisible(bool invisible) {
    QWriteLocker locker(&lock);
    this->invisible = invisible;
}

bool GameDataContainer::getInvisible() {
    QReadLocker locker(&lock);
    return this->invisible;
}

void GameDataContainer::setWebbed(bool webbed) {
    QWriteLocker locker(&lock);
    this->webbed = webbed;
}

bool GameDataContainer::getWebbed() {
    QReadLocker locker(&lock);
    return this->webbed;
}

void GameDataContainer::setJoined(bool joined) {
    QWriteLocker locker(&lock);
    this->joined = joined;
}

bool GameDataContainer::getJoined() {
    QReadLocker locker(&lock);
    return this->joined;
}

void GameDataContainer::setDead(bool dead) {
    QWriteLocker locker(&lock);
    this->dead = dead;
}

bool GameDataContainer::getDead() {
    QReadLocker locker(&lock);
    return this->dead;
}

void GameDataContainer::setHealth(int health) {
    QWriteLocker locker(&lock);
    this->health = health;
}

int GameDataContainer::getHealth() {
    QReadLocker locker(&lock);
    return this->health;
}

void GameDataContainer::setConcentration(int concentration) {
    QWriteLocker locker(&lock);
    this->concentration = concentration;
}

int GameDataContainer::getConcentration() {
    QReadLocker locker(&lock);
    return this->concentration;
}

void GameDataContainer::setSpirit(int spirit) {
    QWriteLocker locker(&lock);
    this->spirit = spirit;
}

int GameDataContainer::getSpirit() {
    QReadLocker locker(&lock);
    return this->spirit;
}

void GameDataContainer::setMana(int mana) {
    QWriteLocker locker(&lock);
    this->mana = mana;
}

int GameDataContainer::getMana() {
    QReadLocker locker(&lock);
    return this->mana;
}

void GameDataContainer::setFatigue(int fatigue) {

    QWriteLocker locker(&lock);
    this->fatigue = fatigue;
}

int GameDataContainer::getFatigue() {
    QReadLocker locker(&lock);
    return this->fatigue;
}

void GameDataContainer::setRt(int rt) {
    if(rt < 0) rt = 0;
    QWriteLocker locker(&lock);
    this->rt = rt;
}

void GameDataContainer::setCt(int ct) {
    if(ct < 0) ct = 0;
    QWriteLocker locker(&lock);
    this->ct = ct;
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
}

void GameDataContainer::clearActiveSpells() {
    QWriteLocker locker(&lock);
    this->activeSpells.clear();
}

QList<QString> GameDataContainer::getDirections() {
    QReadLocker locker(&lock);
    return this->directions;
}

void GameDataContainer::setDirections(QList<QString> directions) {
    QWriteLocker locker(&lock);
    this->directions = directions;
}
