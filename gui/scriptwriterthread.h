#ifndef SCRIPTWRITERTHREAD_H
#define SCRIPTWRITERTHREAD_H

#include <QThread>
#include <QObject>
#include <QQueue>
#include <QMutex>

class ScriptService;

class ScriptWriterThread : public QThread {
    Q_OBJECT

public:
    explicit ScriptWriterThread(QObject *parent = 0);
    ~ScriptWriterThread();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    ScriptService* scriptService;
    QMutex mMutex;
    QString localData;

    QRegExp rxRemoveTags;

    bool exit;

    void process(QString data);
    
signals:
    void writeText(QByteArray);

public slots:
    void addText(QString text);
    
};

#endif // SCRIPTWRITERTHREAD_H
