#ifndef SCRIPTWRITERTHREAD_H
#define SCRIPTWRITERTHREAD_H

#include <QRegExp>
#include <QString>
#include <QByteArray>

#include "workqueuethread.h"

class ScriptService;

class ScriptWriterThread : public WorkQueueThread<QString> {
    Q_OBJECT

public:
    explicit ScriptWriterThread(QObject *parent = 0);
    ~ScriptWriterThread() = default;

    void onProcess(const QString& data) override;

private:
    ScriptService* scriptService;
    QRegExp rxRemoveTags;
    
signals:
    void writeText(QByteArray);    
};

#endif // SCRIPTWRITERTHREAD_H
