#include "expmodel.h"

ExpModel::ExpModel(bool brief, QString expString, QObject *parent) : QObject(parent) {
    this->dataConverterService = DataConverterService::Instance();
    this->expString = expString;
    this->brief = brief;
    this->extractValues();    
}

void ExpModel::extractValues() {
    QStringList exp = expString.trimmed().split(":");

    if(exp.size() == 2) {
        this->name = exp.at(0);
        QStringList expValues = exp.at(1).trimmed().split(" ", QString::SkipEmptyParts);
        if(expValues.size() == 3) {
            this->rank = expValues.at(0).toInt();
            this->rankProgression = expValues.at(1);

            if(brief) {
                int value = briefNumeric(expValues.at(2));
                this->state = dataConverterService->expNumericToState(value);
                this->numericState = value;
            } else {
                this->state = expValues.at(2);
                this->numericState = dataConverterService->expStateToNumeric(this->state);
            }
        }
    }
}

int ExpModel::briefNumeric(QString text) {
    QRegExp rx("(\\d+)");
    rx.indexIn(text, 0);
    return rx.cap(1).toInt();
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
           ", state => " + this->state +
           ", numericState => " + QString::number(this->numericState) + " ]";
}
