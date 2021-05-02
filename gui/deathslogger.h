#ifndef DEATHSLOGGER_H
#define DEATHSLOGGER_H

#include <QString>

#include <log4qt/logger.h>

#include "workqueuethread.h"

class DeathsLogger : public WorkQueueThread<QString> {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    
    using Parent = WorkQueueThread<QString>;
public:
    explicit DeathsLogger(QObject *parent = 0);
    ~DeathsLogger() = default;

protected:
    void onProcess(const QString& text) override;
    
public slots:
    void addText(QString);

};

#endif // DEATHSLOGGER_H
