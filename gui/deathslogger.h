#ifndef DEATHSLOGGER_H
#define DEATHSLOGGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>

#include <log4qt/logger.h>

class DeathsLogger : public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit DeathsLogger(QObject *parent = 0);
    ~DeathsLogger();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QMutex mMutex;
    QString localData;

signals:

public slots:
    void addText(QString);

};

#endif // DEATHSLOGGER_H
