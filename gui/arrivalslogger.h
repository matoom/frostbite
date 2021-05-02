#ifndef ARRIVALSLOGGER_H
#define ARRIVALSLOGGER_H

#include <QString>
#include <QRegExp>

#include <log4qt/logger.h>

#include "workqueuethread.h"

class ArrivalsLogger : public WorkQueueThread<QString> {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    using Parent = WorkQueueThread<QString>;
public:
    explicit ArrivalsLogger(QObject *parent = 0);
    ~ArrivalsLogger() = default;
    
protected:
    void onProcess(const QString& text);

signals:

public slots:
    void addText(QString);

};


#endif // ARRIVALSLOGGER_H
