#ifndef DATACONVERSIONSERVICE_H
#define DATACONVERSIONSERVICE_H

#include <QObject>
#include <QDebug>
#include <QHash>
#include <QStringList>

class DataConverterService : public QObject {
    Q_OBJECT

public:
    static DataConverterService* Instance();

    int expStateToNumeric(QString);
    QString expNumericToState(int);
    QString msToMMSS(int);

private:
    DataConverterService(QObject *parent = 0);
    DataConverterService(DataConverterService const& copy);
    DataConverterService& operator = (DataConverterService const& copy);
    static DataConverterService* m_pInstance;

    void populateExpStates();
    QStringList mindStates;

signals:
    
public slots:
    
};

#endif // DATACONVERSIONSERVICE_H
