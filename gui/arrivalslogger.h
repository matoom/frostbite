#ifndef ARRIVALSLOGGER_H
#define ARRIVALSLOGGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>

#include <log4qt/logger.h>

class ArrivalsLogger : public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit ArrivalsLogger(QObject *parent = 0);
    ~ArrivalsLogger();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QMutex mMutex;
    QString localData;

signals:

public slots:
    void addText(QString);

};


#endif // ARRIVALSLOGGER_H
