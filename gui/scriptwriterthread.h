#ifndef SCRIPTWRITERTHREAD_H
#define SCRIPTWRITERTHREAD_H

#include <QThread>
#include <QObject>
#include <QQueue>
#include <QMutex>
#include "concurrentqueue.h"

class ScriptService;

class ScriptWriterThread : public QThread {
    Q_OBJECT

public:
    explicit ScriptWriterThread(QObject *parent = 0);
    ~ScriptWriterThread();

    virtual void run();

private:
    ConcurrentQueue<QString> dataQueue;
    ScriptService* scriptService;
    QRegExp rxRemoveTags;

    void process(const QString& data);
    
signals:
    void writeText(QByteArray);

public slots:
    void addText(QString text);
    
};

#endif // SCRIPTWRITERTHREAD_H
