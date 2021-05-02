#ifndef THOUGHTSLOGGER_H
#define THOUGHTSLOGGER_H

#include <QString>
#include <QRegExp>

#include <log4qt/logger.h>

#include "workqueuethread.h"

class ThoughtsLogger : public WorkQueueThread<QString> {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    using Parent = WorkQueueThread<QString>;
public:
    explicit ThoughtsLogger(QObject *parent = 0);
    ~ThoughtsLogger() = default;

protected:
    void onProcess(const QString& text) override {
        log(text);
    }
    

private:
    QRegExp rxRemoveTags;

    void log(QString);

signals:

public slots:
    void addText(QString);

};

#endif // THOUGHTSLOGGER_H
