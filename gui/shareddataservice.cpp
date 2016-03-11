#include "shareddataservice.h"

SharedDataService* SharedDataService::m_pInstance = NULL;

SharedDataService* SharedDataService::Instance() {
    if (!m_pInstance) {
        m_pInstance = new SharedDataService;
    }
    return m_pInstance;
}

SharedDataService::SharedDataService(QObject *parent) : QObject(parent) {
    this->loadDataLib();    
}

bool SharedDataService::isLoaded() {
    return loaded;
}

void SharedDataService::loadDataLib() {
    dataLib = new QLibrary();
    dataLib->setFileName(QApplication::applicationDirPath() + "/shared");
    loaded = dataLib->load();

    addExpField = (AddExp)dataLib->resolve("setExpField");
    removeExpField = (RemoveExp)dataLib->resolve("removeExpField");

    setStanding = (Set_b)dataLib->resolve("setStanding");
    setSitting = (Set_b)dataLib->resolve("setSitting");
    setKneeling = (Set_b)dataLib->resolve("setKneeling");
    setProne = (Set_b)dataLib->resolve("setProne");
    setStunned = (Set_b)dataLib->resolve("setStunned");
    setBleeding = (Set_b)dataLib->resolve("setBleeding");
    setHidden = (Set_b)dataLib->resolve("setHidden");
    setInvisible = (Set_b)dataLib->resolve("setInvisible");
    setWebbed = (Set_b)dataLib->resolve("setWebbed");
    setJoined = (Set_b)dataLib->resolve("setJoined");
    setDead = (Set_b)dataLib->resolve("setDead");

    setActiveSpells = (Set_s)dataLib->resolve("setActiveSpells");

    setInventory = (Set_s)dataLib->resolve("setInventory");

    setContainer = (Set_s)dataLib->resolve("setContainer");

    setWieldRight = (Set_s)dataLib->resolve("setWieldRight");
    setWieldRightNoun = (Set_s)dataLib->resolve("setWieldRightNoun");
    setWieldLeft = (Set_s)dataLib->resolve("setWieldLeft");
    setWieldLeftNoun = (Set_s)dataLib->resolve("setWieldLeftNoun");

    setHealth = (Set_i)dataLib->resolve("setHealth");
    setConcentration = (Set_i)dataLib->resolve("setConcentration");
    setSpirit = (Set_i)dataLib->resolve("setSpirit");
    setFatigue = (Set_i)dataLib->resolve("setFatigue");

    setRoomTitle = (Set_s)dataLib->resolve("setRoomTitle");
    setRoomDescription = (Set_s)dataLib->resolve("setRoomDescription");
    setRoomObjects = (Set_s)dataLib->resolve("setRoomObjects");
    setRoomPlayers = (Set_s)dataLib->resolve("setRoomPlayers");
    setRoomExits = (Set_s)dataLib->resolve("setRoomExits");

    setRt = (Set_i)dataLib->resolve("setRt");

    //destroy = (Destroy)dataLib->resolve("destroy");
}

/*SharedDataService::~SharedDataService() {
    destroy();
    dataLib->unload();
    delete dataLib;
}*/
