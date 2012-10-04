#include "dataconverterservice.h"

DataConverterService* DataConverterService::m_pInstance = NULL;

DataConverterService* DataConverterService::Instance() {
    if (!m_pInstance) {
        m_pInstance = new DataConverterService;
    }

    return m_pInstance;
}

DataConverterService::DataConverterService(QObject *parent) : QObject(parent) {
    this->populateExpStates();
}

void DataConverterService::populateExpStates() {
    expStates.insert("clear", 0);
    expStates.insert("dabbling", 1);
    expStates.insert("perusing", 2);
    expStates.insert("learning", 3);
    expStates.insert("thoughtful", 4);
    expStates.insert("thinking", 5);
    expStates.insert("considering", 6);
    expStates.insert("pondering", 7);
    expStates.insert("ruminating", 8);
    expStates.insert("dabbling", 9);
    expStates.insert("concentrating", 10);
    expStates.insert("attentive", 11);
    expStates.insert("deliberative", 12);
    expStates.insert("interested", 13);
    expStates.insert("examining", 14);
    expStates.insert("understanding", 15);
    expStates.insert("absorbing", 16);
    expStates.insert("intrigued", 17);
    expStates.insert("scrutinizing", 18);
    expStates.insert("analyzing", 19);
    expStates.insert("studious", 20);
    expStates.insert("focused", 21);
    expStates.insert("very focused", 22);
    expStates.insert("engaged", 23);
    expStates.insert("very engaged", 24);
    expStates.insert("cogitating", 25);
    expStates.insert("fascinated", 26);
    expStates.insert("engrossed", 27);
    expStates.insert("riveted", 28);
    expStates.insert("very riveted", 29);
    expStates.insert("rapt", 30);
    expStates.insert("very rapt", 31);
    expStates.insert("enthralled", 32);
    expStates.insert("nearly locked", 33);
    expStates.insert("mind lock", 34);
}

int DataConverterService::expStateToNumeric(QString state) {
    return expStates.value(state);
}

QString DataConverterService::msToMMSS(int ms) {
    int ss = ms / 1000;
    int mm = ss / 60;
    ss = ss % 60;
    return QString().sprintf("%02d:%02d", mm, ss);
    //return QString("%1:%2").arg(mm).arg(ss);
}
