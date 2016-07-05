#ifndef MAINLOGGER_H
#define MAINLOGGER_H

#include <QObject>
#include <QQueue>
#include <QDebug>
#include <QMutex>
#include <QRegExp>
#include <QThread>

#include <log4qt/logger.h>

#include <textutils.h>

class MainLogger : public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

struct LogEntry {
    QString text;
    char type;
};

public:
    explicit MainLogger(QObject *parent = 0);
    ~MainLogger();

    virtual void run();

    static const char COMMAND = 'c';
    static const char PROMPT = 'p';

private:
    QQueue<LogEntry> dataQueue;
    QMutex mMutex;
    QRegExp rxRemoveTags;
    LogEntry localData;

    void log(LogEntry);

    char prevType;

signals:
    
public slots:
    void addText(QString, char type = '\0');

};

#endif // MAINLOGGER_H
