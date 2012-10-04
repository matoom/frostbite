#ifndef DATACONVERSIONSERVICE_H
#define DATACONVERSIONSERVICE_H

#include <QObject>
#include <QDebug>
#include <QHash>

class DataConverterService : public QObject {
    Q_OBJECT

public:
    static DataConverterService* Instance();

    int expStateToNumeric(QString);
    QString msToMMSS(int);

private:
    DataConverterService(QObject *parent = 0);
    DataConverterService(DataConverterService const& copy);
    DataConverterService& operator = (DataConverterService const& copy);
    static DataConverterService* m_pInstance;

    void populateExpStates();
    QHash<QString, int> expStates;

signals:
    
public slots:
    
};

#endif // DATACONVERSIONSERVICE_H
