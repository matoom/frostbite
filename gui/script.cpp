#include "script.h"

Script::Script(QObject *parent) : QObject(parent), script_proc(new QProcess(this)) {
    scriptService = (ScriptService*)parent;
    clientSettings = ClientSettings::Instance();

    connect(script_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(displayOutputMsg()));
    connect(script_proc, SIGNAL(readyReadStandardError()), this, SLOT(displayErrorMsg()));
    connect(script_proc, SIGNAL(started()), this, SLOT(start()));
    connect(script_proc, SIGNAL(finished(int)), this, SLOT(finish(int)));

    #if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        connect(script_proc, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
    #else
        connect(script_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
    #endif

    path = QApplication::applicationDirPath() + "/scripts/lib/main.rb";

    rubyPath = clientSettings->getParameter("Ruby/path", "ruby").toString();

    running = false;
}

bool Script::isRunning() {
    QReadLocker locker(&lock);
    return script_proc->state() == QProcess::Running;
}

QString Script::currentFileName() {
    return this->fileName;
}

void Script::execute(QString fileName, QList<QString> userArgs) {   
    QString file = QApplication::applicationDirPath() + "/scripts/" + fileName + ".rb";
    this->fileName = fileName;

    QStringList arguments;
    arguments << path << file << userArgs;

    script_proc->start(rubyPath, arguments, QProcess::Unbuffered | QProcess::ReadWrite);
}

void Script::killScript() {
    procMutex.lock();
    script_proc->kill();
    script_proc->waitForFinished(1000);
    procMutex.unlock();
}

void Script::sendMessage(QByteArray message) {
    procMutex.lock();
    if(script_proc->isOpen() && script_proc->isWritable()) {
        script_proc->write(message);
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        script_proc->waitForBytesWritten(-1);
        #endif
    }
    procMutex.unlock();
}

void Script::displayOutputMsg() {
    procMutex.lock();
    script_proc->setReadChannel(QProcess::StandardOutput);
    QByteArray msg = script_proc->readAll();        
    procMutex.unlock();
    scriptService->processCommand(msg);
    //qDebug() << msg.data();
}

void Script::displayErrorMsg() {
    procMutex.lock();
    script_proc->setReadChannel(QProcess::StandardError);
    QByteArray msg = script_proc->readAll();
    procMutex.unlock();
    //msg.prepend("@SCRIPT ERROR\n");
    scriptService->writeGameWindow(msg);
    //qDebug() << "Error: " << (msg.data());
}

void Script::start() {
}

void Script::finish(int exit) {
    procMutex.lock();
    script_proc->closeWriteChannel();
    procMutex.unlock();

    if(exit == 0) {
        scriptService->scriptFinished();
    }
    scriptService->scriptEnded();    
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
