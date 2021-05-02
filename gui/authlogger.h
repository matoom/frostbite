#ifndef AUTHLOGGER_H
#define AUTHLOGGER_H

#include <QString>
#include <QRegExp>

#include <log4qt/logger.h>

#include "workqueuethread.h"

class AuthLogger: public WorkQueueThread<QString> {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    
    using Parent = WorkQueueThread<QString>;
public:
    explicit AuthLogger(QObject *parent = 0);
    ~AuthLogger() = default;

protected:
    void onProcess(const QString& text) override;
    
signals:

public slots:
    void addText(QString);

};

#endif // AUTHLOGGER_H
