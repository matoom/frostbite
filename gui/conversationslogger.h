#ifndef CONVERSATIONSLOGGER_H
#define CONVERSATIONSLOGGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QRegExp>

#include <log4qt/logger.h>

class ConversationsLogger : public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit ConversationsLogger(QObject *parent = 0);
    ~ConversationsLogger();

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
#endif // CONVERSATIONSLOGGER_H
