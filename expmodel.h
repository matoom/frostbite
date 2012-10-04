#ifndef EXPMODEL_H
#define EXPMODEL_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <QList>
#include <QColor>
#include <QBitArray>

#include <dataconverterservice.h>

class DataConverterService;

class ExpModel : public QObject {
    Q_OBJECT

public:
    ExpModel(QString expString, QObject *parent = 0);

    DataConverterService *dataConverterService;

    QString name;
    int rank;
    QString rankProgression;
    QString state;
    int numericState;

    QString expString;

    QString getName();
    int getRank();
    QString getRankProgression();
    QString getState();
    QString getExpString();
    int getNumericState();

    QString toString();

private:
    void extractValues();

signals:
    
public slots:
    
};

#endif // EXPMODEL_H
