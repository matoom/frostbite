#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>

#include <log4qt/logger.h>

class DebugLogger: public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit DebugLogger(QObject *parent = 0);
    ~DebugLogger();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QMutex mMutex;
    QString localData;

signals:

public slots:
    void addText(QString);

};

#endif // DEBUGLOGGER_H
