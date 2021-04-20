#ifndef MAINLOGGER_H
#define MAINLOGGER_H

#include <QDebug>
#include <QRegExp>

#include <log4qt/logger.h>

#include "workqueuethread.h"


class Alter;

struct LogEntry {
    QString text;
    char type;
};

class MainLogger : public WorkQueueThread<LogEntry> {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    
    using Parent = WorkQueueThread<LogEntry>;
public:
    explicit MainLogger(QObject *parent = 0);
    ~MainLogger() = default;

    static const char COMMAND = 'c';
    static const char PROMPT = 'p';
protected:
    void onProcess(const LogEntry& entry) override;
    
private:
    QRegExp rxRemoveTags;
    Alter* alter;

    void log(LogEntry);

    char prevType;

signals:
    
public slots:
    void addText(QString, char type = '\0');
    void updateSettings();

};

#endif // MAINLOGGER_H
