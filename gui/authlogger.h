#ifndef AUTHLOGGER_H
#define AUTHLOGGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>

#include <log4qt/logger.h>

class AuthLogger: public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit AuthLogger(QObject *parent = 0);
    ~AuthLogger();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QMutex mMutex;
    QString localData;

signals:

public slots:
    void addText(QString);

};

#endif // AUTHLOGGER_H
