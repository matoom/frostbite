#include "expmodel.h"

ExpModel::ExpModel(QString expString, QObject *parent) : QObject(parent) {
    this->dataConverterService = DataConverterService::Instance();
    this->expString = expString;
    this->extractValues();
}

void ExpModel::extractValues() {
    QStringList exp = expString.trimmed().split(":");

    if(exp.size() == 2) {
        this->name = exp.at(0);
        QStringList expValues = exp.at(1).trimmed().split(" ");
        if(expValues.size() == 3) {
            this->rank = expValues.at(0).toInt();
            this->rankProgression = expValues.at(1);
            this->state = expValues.at(2);
            this->numericState = dataConverterService->expStateToNumeric(this->state);
        }
    }
}

QString ExpModel::getName() {
    return this->name;
}

int ExpModel::getRank() {
    return this->rank;
}

QString ExpModel::getRankProgression() {
    return this->rankProgression;
}

QString ExpModel::getState() {
    return this->state;
}

int ExpModel::getNumericState() {
    return this->numericState;
}

QString ExpModel::getExpString() {
    return this->expString;
}

QString ExpModel::toString() {
    return "ExpModel:[ name => " + this->name  +
            ", rank => " + QString::number(this->rank)  +
            ", state => " + this->state + " ]";
}
