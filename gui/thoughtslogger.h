#ifndef THOUGHTSLOGGER_H
#define THOUGHTSLOGGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QRegExp>
#include <QThread>

#include <log4qt/logger.h>
#include <textutils.h>

class ThoughtsLogger : public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit ThoughtsLogger(QObject *parent = 0);
    ~ThoughtsLogger();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QMutex mMutex;
    QRegExp rxRemoveTags;
    QString localData;

    void log(QString);

signals:

public slots:
    void addText(QString);

};

#endif // THOUGHTSLOGGER_H
