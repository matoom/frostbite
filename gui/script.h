#ifndef SCRIPT_H
#define SCRIPT_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QDir>

#include <scriptservice.h>

class ScriptService;

class Script : public QObject {
    Q_OBJECT

public:
    Script(QObject *parent = 0);
    ~Script();

    void execute(QString, QList<QString>);

    void killScript();
    bool isRunning();
    void sendMessage(QByteArray);    
    QString currentFileName();

private:
    ScriptService* scriptService;
    QProcess* script_proc;

    QString path;
    QString fileName;

    bool running;

signals:

public slots:

private slots:
    void displayOutputMsg();
    void displayErrorMsg();
    void start();
    void finish(int);
    void handleError(QProcess::ProcessError);
};

#endif // SCRIPT_H
