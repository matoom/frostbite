#include "shared.h"

QString locationId = QString(QCryptographicHash::hash(QDir::currentPath().toLocal8Bit(), QCryptographicHash::Md5).toHex());

QMap<QString, QSharedMemory*> initStringMemoryMap() {
    QMap<QString, QSharedMemory*> stringMemoryMap;
    stringMemoryMap.insert(ROOM_TITLE_SHARED_NAME, new QSharedMemory(ROOM_TITLE_SHARED_NAME));
    stringMemoryMap.insert(ROOM_DESC_SHARED_NAME, new QSharedMemory(ROOM_DESC_SHARED_NAME));
    stringMemoryMap.insert(ROOM_OBJECTS_SHARED_NAME, new QSharedMemory(ROOM_OBJECTS_SHARED_NAME));
    stringMemoryMap.insert(ROOM_PLAYERS_SHARED_NAME, new QSharedMemory(ROOM_PLAYERS_SHARED_NAME));
    stringMemoryMap.insert(ROOM_EXITS_SHARED_NAME, new QSharedMemory(ROOM_EXITS_SHARED_NAME));
    stringMemoryMap.insert(INVENTORY_SHARED_NAME, new QSharedMemory(INVENTORY_SHARED_NAME));
    stringMemoryMap.insert(CONTAINER_SHARED_NAME, new QSharedMemory(CONTAINER_SHARED_NAME));
    stringMemoryMap.insert(WIELD_RIGHT_SHARED_NAME, new QSharedMemory(WIELD_RIGHT_SHARED_NAME));
    stringMemoryMap.insert(WIELD_RIGHT_NOUN_SHARED_NAME, new QSharedMemory(WIELD_RIGHT_NOUN_SHARED_NAME));
    stringMemoryMap.insert(WIELD_LEFT_SHARED_NAME, new QSharedMemory(WIELD_LEFT_SHARED_NAME));
    stringMemoryMap.insert(WIELD_LEFT_NOUN_SHARED_NAME, new QSharedMemory(WIELD_LEFT_NOUN_SHARED_NAME));
    return stringMemoryMap;
}

QMap<QString, QSharedMemory*> initIntMemoryMap() {
    QMap<QString, QSharedMemory*> intMemoryMap;
    intMemoryMap.insert(HEALTH_SHARED_NAME, new QSharedMemory(HEALTH_SHARED_NAME));
    intMemoryMap.insert(CONCENTRATION_SHARED_NAME, new QSharedMemory(CONCENTRATION_SHARED_NAME));
    intMemoryMap.insert(SPIRIT_SHARED_NAME, new QSharedMemory(SPIRIT_SHARED_NAME));
    intMemoryMap.insert(FATIGUE_SHARED_NAME, new QSharedMemory(FATIGUE_SHARED_NAME));
    intMemoryMap.insert(RT_SHARED_NAME, new QSharedMemory(RT_SHARED_NAME));
    return intMemoryMap;
}

QMap<QString, QSharedMemory*> initBoolMemoryMap() {
    QMap<QString, QSharedMemory*> boolMemoryMap;
    boolMemoryMap.insert(STANDING_SHARED_NAME, new QSharedMemory(STANDING_SHARED_NAME));
    boolMemoryMap.insert(SITTING_SHARED_NAME, new QSharedMemory(SITTING_SHARED_NAME));
    boolMemoryMap.insert(KNEELING_SHARED_NAME, new QSharedMemory(KNEELING_SHARED_NAME));
    boolMemoryMap.insert(PRONE_SHARED_NAME, new QSharedMemory(PRONE_SHARED_NAME));
    boolMemoryMap.insert(STUNNED_SHARED_NAME, new QSharedMemory(STUNNED_SHARED_NAME));
    boolMemoryMap.insert(BLEEDING_SHARED_NAME, new QSharedMemory(BLEEDING_SHARED_NAME));
    boolMemoryMap.insert(HIDDEN_SHARED_NAME, new QSharedMemory(HIDDEN_SHARED_NAME));
    boolMemoryMap.insert(INVISIBLE_SHARED_NAME, new QSharedMemory(INVISIBLE_SHARED_NAME));
    boolMemoryMap.insert(WEBBED_SHARED_NAME, new QSharedMemory(WEBBED_SHARED_NAME));
    boolMemoryMap.insert(JOINED_SHARED_NAME, new QSharedMemory(JOINED_SHARED_NAME));
    boolMemoryMap.insert(DEAD_SHARED_NAME, new QSharedMemory(DEAD_SHARED_NAME));
    return boolMemoryMap;
}

QMap<QString, QSharedMemory*> boolMemoryMap = initBoolMemoryMap();
QMap<QString, QSharedMemory*> intMemoryMap = initIntMemoryMap();
QMap<QString, QSharedMemory*> stringMemoryMap = initStringMemoryMap();
QSharedMemory *expShm = new QSharedMemory(EXP_SHARED_NAME);

QMap<QString, QMap<QString, int> > readExpMap() {
    QBuffer buffer;
    QDataStream in(&buffer);
    QMap<QString, QMap<QString, int> > expMap;

    expShm->attach(QSharedMemory::ReadWrite);

    if(!expShm->isAttached()) {
        return expMap;
    }

    expShm->lock();

    buffer.setData((char*)expShm->constData(), expShm->size());

    buffer.open(QBuffer::ReadOnly);
    in >> expMap;
    buffer.close();

    expShm->unlock();

    expShm->detach();

    return expMap;
}

void setExp(const char name[], QMap<QString, int> expValueMap) {
    QBuffer buffer;
    QDataStream in(&buffer);
    QMap<QString, QMap<QString, int> > expMap = readExpMap();
    expMap[name] = expValueMap;

    buffer.open(QBuffer::ReadWrite);
    in << expMap;

    if(expShm->isAttached())
        expShm->detach();

    if (!expShm->create(buffer.size() + sizeof(char))) {
        expShm->attach(QSharedMemory::ReadWrite);
    }

    QBuffer bufferwrite;
    QDataStream out(&bufferwrite);

    expShm->lock();

    bufferwrite.open(QBuffer::ReadWrite);
    out << expMap;
    bufferwrite.close();

    char *to = (char*)expShm->data();
    const char *from = bufferwrite.data().data();
    memcpy(to, from, qMin(expShm->size(), (int)bufferwrite.size()));

    expShm->unlock();
}

QMap<QString, int> getExp(const char name[]) {
    QMap<QString, QMap<QString, int> > expMap = readExpMap();

    QMap<QString, int> expValueMap;

    QMap<QString, QMap<QString, int> >::iterator find = expMap.find(name);
    if(find != expMap.end()) {
        expValueMap = find.value();
    } else {
        QMapIterator<QString, QMap<QString, int> > nearestValue(expMap);
        while (nearestValue.hasNext()) {
            nearestValue.next();
            size_t nameSize = strlen(name);
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

void setString(const char value[], QString name) {
    QSharedMemory* sharedMemory = stringMemoryMap.value(name);

    if(sharedMemory->isAttached())
        sharedMemory->detach();

    int size = strlen(value) + sizeof(char);

    if (!sharedMemory->create(size)) {
        if (!sharedMemory->isAttached()) {
            sharedMemory->attach(QSharedMemory::ReadWrite);
        }
    }

    sharedMemory->lock();

    char *to = (char*)sharedMemory->data();
    memcpy(to, value, qMin(sharedMemory->size(), size));

    sharedMemory->unlock();
}

char* getString(QString name) {
    QSharedMemory* sharedMemory = stringMemoryMap[name];

    sharedMemory->attach(QSharedMemory::ReadOnly);

    char* strValue = (char*)"";

    if(!sharedMemory->isAttached()) {
        return strValue;
    }

    sharedMemory->lock();

    char* shared = (char*)sharedMemory->data();
    strValue = new char[strlen(shared) + sizeof(char)];
    strcpy(strValue, shared);

    sharedMemory->unlock();

    sharedMemory->detach();

    return strValue;
}

void setInt(int value, QString name) {
    QSharedMemory* sharedMemory = intMemoryMap.value(name);

    if(sharedMemory->isAttached())
        sharedMemory->detach();

    int size = sizeof(int);

    if (!sharedMemory->create(size)) {
        if (!sharedMemory->isAttached()) {
            sharedMemory->attach(QSharedMemory::ReadWrite);
        }
    }

    sharedMemory->lock();

    int* to = (int*)sharedMemory->data();

    memcpy(to, &value, qMin(sharedMemory->size(), size));

    sharedMemory->unlock();
}

int getInt(QString name) {
    QSharedMemory* sharedMemory = intMemoryMap[name];

    sharedMemory->attach(QSharedMemory::ReadOnly);

    int returnValue = 0;

    if(!sharedMemory->isAttached()) {
        return returnValue;
    }

    sharedMemory->lock();

    int* value = (int*)sharedMemory->data();
    memcpy(&returnValue, value, sizeof(value));

    sharedMemory->unlock();

    sharedMemory->detach();

    return returnValue;
}

void setBool(bool value, QString name) {
    QSharedMemory* sharedMemory = boolMemoryMap.value(name);

    if(sharedMemory->isAttached()) {
        sharedMemory->detach();
    }

    int size = sizeof(bool);

    if (!sharedMemory->create(size)) {
        sharedMemory->attach(QSharedMemory::ReadWrite);
    }

    sharedMemory->lock();

    bool *array = (bool*)sharedMemory->data();
    memcpy(array, &value, qMin(sharedMemory->size(), size));

    sharedMemory->unlock();
}

bool getBool(QString name) {
    QSharedMemory* sharedMemory = boolMemoryMap[name];

    sharedMemory->attach(QSharedMemory::ReadOnly);

    bool returnValue = false;

    if(!sharedMemory->isAttached()) {
        return returnValue;
    }

    sharedMemory->lock();

    bool value = *(bool*)sharedMemory->data();

    returnValue = value;

    sharedMemory->unlock();

    sharedMemory->detach();

    return returnValue;
}

// exp
extern "C" EXPORT_FUNCTION void setExpField(const char name[], int rank, int state) {
    QMap<QString, int> expValueMap;
    expValueMap["rank"] = rank;
    expValueMap["state"] = state;

    setExp(name, expValueMap);
}

extern "C" EXPORT_FUNCTION void removeExpField(const char name[]) {
    QMap<QString, int> expValueMap;
    expValueMap["rank"] = 0;
    expValueMap["state"] = 0;

    setExp(name, expValueMap);
}

extern "C" EXPORT_FUNCTION int getExpRank(const char name[]) {
    return getExp(name).value("rank");
}

extern "C" EXPORT_FUNCTION int getExpState(const char name[]) {
    return getExp(name).value("state");
}

// status
extern "C" EXPORT_FUNCTION bool getStanding() {
    return getBool(STANDING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setStanding(bool standing) {
    setBool(standing, STANDING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getSitting() {
    return getBool(SITTING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setSitting(bool sitting) {
    setBool(sitting, SITTING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getKneeling() {
    return getBool(KNEELING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setKneeling(bool kneeling) {
    setBool(kneeling, KNEELING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getProne() {
    return getBool(PRONE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setProne(bool prone) {
    setBool(prone, PRONE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getStunned() {
    return getBool(STUNNED_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setStunned(bool stunned) {
    setBool(stunned, STUNNED_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getBleeding() {
    return getBool(BLEEDING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setBleeding(bool bleeding) {
    setBool(bleeding, BLEEDING_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getHidden() {
    return getBool(HIDDEN_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setHidden(bool hidden) {
    setBool(hidden, HIDDEN_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getInvisible() {
    return getBool(INVISIBLE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setInvisible(bool invisible) {
    setBool(invisible, INVISIBLE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getWebbed() {
    return getBool(WEBBED_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setWebbed(bool webbed) {
    setBool(webbed, WEBBED_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getJoined() {
    return getBool(JOINED_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setJoined(bool joined) {
    setBool(joined, JOINED_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION bool getDead() {
    return getBool(DEAD_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setDead(bool dead) {
    setBool(dead, DEAD_SHARED_NAME);
}

// inventory
extern "C" EXPORT_FUNCTION char* getInventory() {
    return getString(INVENTORY_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setInventory(const char inv[]) {
    setString(inv, INVENTORY_SHARED_NAME);
}

// container
extern "C" EXPORT_FUNCTION char* getContainer() {
    return getString(CONTAINER_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setContainer(const char cont[]) {
    setString(cont, CONTAINER_SHARED_NAME);
}

// wield

extern "C" EXPORT_FUNCTION char* getWieldRight() {
    return getString(WIELD_RIGHT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setWieldRight(const char wield[]) {
    setString(wield, WIELD_RIGHT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getWieldRightNoun() {
    return getString(WIELD_RIGHT_NOUN_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setWieldRightNoun(const char wield[]) {
    setString(wield, WIELD_RIGHT_NOUN_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getWieldLeft() {
    return getString(WIELD_LEFT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setWieldLeft(const char wield[]) {
    setString(wield, WIELD_LEFT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getWieldLeftNoun() {
    return getString(WIELD_LEFT_NOUN_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setWieldLeftNoun(const char wield[]) {
    setString(wield, WIELD_LEFT_NOUN_SHARED_NAME);
}

// vitals
extern "C" EXPORT_FUNCTION int getHealth() {
    return getInt(HEALTH_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setHealth(int value) {
    setInt(value, HEALTH_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION int getConcentration() {
    return getInt(CONCENTRATION_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setConcentration(int value) {
    setInt(value, CONCENTRATION_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION int getSpirit() {
    return getInt(SPIRIT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setSpirit(int value) {
   setInt(value, SPIRIT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION int getFatigue() {
    return getInt(FATIGUE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setFatigue(int value) {
   setInt(value, FATIGUE_SHARED_NAME);
}

// room
extern "C" EXPORT_FUNCTION char* getRoomTitle() {
    return getString(ROOM_TITLE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setRoomTitle(const char title[]) {    
    setString(title, ROOM_TITLE_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getRoomDescription() { 
    return getString(ROOM_DESC_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setRoomDescription(const char desc[]) {
    setString(desc, ROOM_DESC_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getRoomObjects() {
    return getString(ROOM_OBJECTS_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setRoomObjects(const char obj[]) {
    setString(obj, ROOM_OBJECTS_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getRoomPlayers() {
    return getString(ROOM_PLAYERS_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setRoomPlayers(const char players[]) {
    setString(players, ROOM_PLAYERS_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION char* getRoomExits() {
    return getString(ROOM_EXITS_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION void setRoomExits(const char exits[]) {
    setString(exits, ROOM_EXITS_SHARED_NAME);
}

// rt
extern "C" EXPORT_FUNCTION void setRt(int value) {
    setInt(value, RT_SHARED_NAME);
}

extern "C" EXPORT_FUNCTION int getRt() {
    return getInt(RT_SHARED_NAME);
}
