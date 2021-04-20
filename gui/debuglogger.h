#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include <QString>

#include <log4qt/logger.h>

#include "workqueuethread.h"

class DebugLogger: public WorkQueueThread<QString> {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    using Parent = WorkQueueThread<QString>;
public:
    explicit DebugLogger(QObject *parent = 0);
    ~DebugLogger() = default;
    
protected:
    void onProcess(const QString& text) override;
    
public slots:
    void addText(QString);

};

#endif // DEBUGLOGGER_H
