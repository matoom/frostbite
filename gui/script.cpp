#include "script.h"

Script::Script(QObject *parent) : QObject(parent), script_proc(new QProcess(this)) {
    scriptService = (ScriptService*)parent;

    connect(script_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(displayOutputMsg()));
    connect(script_proc, SIGNAL(readyReadStandardError()), this, SLOT(displayErrorMsg()));
    connect(script_proc, SIGNAL(started()), this, SLOT(start()));
    connect(script_proc, SIGNAL(finished(int)), this, SLOT(finish(int)));
    connect(script_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));

    path = QDir::currentPath() + "/scripts/lib/main.rb";

    running = false;
}

bool Script::isRunning() {
    return running;
}

QString Script::currentFileName() {
    return this->fileName;
}

void Script::execute(QString fileName, QList<QString> userArgs) {   
    QString file = QDir::currentPath() + "/scripts/" + fileName + ".rb";
    this->fileName = fileName;

    QStringList arguments;
    arguments << "-w" << path << file << userArgs;

    QString program = "ruby";

    script_proc->start(program, arguments);
}

void Script::killScript() {
    script_proc->kill();
    script_proc->waitForFinished(1000);
}

void Script::sendMessage(QByteArray message) {
    script_proc->write(message);
}

void Script::displayOutputMsg() {
    script_proc->setReadChannel(QProcess::StandardOutput);
    QByteArray msg = script_proc->readAll();        
    scriptService->processCommand(msg);
    //qDebug() << msg.data();
}

void Script::displayErrorMsg() {
    script_proc->setReadChannel(QProcess::StandardError);
    QByteArray msg = script_proc->readAll();
    msg.prepend("@SCRIPT ERROR\n");
    scriptService->writeGameWindow(msg);
    //qDebug() << "Error: " << (msg.data());
}

void Script::start() {
    running = true;
}

void Script::finish(int exit) {
    script_proc->closeWriteChannel();

    if(exit == 0) {
        scriptService->scriptFinished();
    }
    scriptService->scriptEnded();

    running = false;
}

void Script::handleError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) {
        scriptService->writeGameWindow("The script process failed to start. "
                                       "Either the Ruby installation is missing, "
                                       "or you may have insufficient permissions to "
                                       "invoke Ruby installation.");
    }
    scriptService->scriptEnded();

    running = false;
}

Script::~Script() {
    delete script_proc;
}
