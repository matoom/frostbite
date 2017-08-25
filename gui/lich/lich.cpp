#include "lich.h"

Lich::Lich(QObject *parent) : QObject(parent), lich_proc(new QProcess(this)) {
    mainWindow = (MainWindow*)parent;
    windowFacade = mainWindow->getWindowFacade();

    clientSettings = ClientSettings::getInstance();

    connect(lich_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(displayOutputMsg()));
    connect(lich_proc, SIGNAL(readyReadStandardError()), this, SLOT(displayErrorMsg()));
    connect(lich_proc, SIGNAL(started()), this, SLOT(start()));
    connect(lich_proc, SIGNAL(finished(int)), this, SLOT(finish(int)));
    connect(lich_proc, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
}

void Lich::run(QString host, QString port) {
    QString lichLocation = clientSettings->getQStringNotBlank("Script/lichLocation", SCRIPT_LICH_LOCATION);
    QString ruby = clientSettings->getQStringNotBlank("Script/lichRuby", SCRIPT_LICH_RUBY);
    QString lichArgs = clientSettings->getQStringNotBlank("Script/lichArguments", SCRIPT_LICH_ARGS);

    QStringList arguments;
    arguments << lichLocation;
    arguments << lichArgs.replace("$host", host).replace("$port", port).split(" ");

    windowFacade->writeGameWindow(QString("<br/>--- Open \"" + ruby + " " + arguments.join(" ")).toLocal8Bit() + "\"<br/>");

    lich_proc->start(ruby, arguments, QProcess::Unbuffered | QProcess::ReadWrite);
}

void Lich::waitUntilRunning() {
    if(!lich_proc->waitForStarted()) {
       windowFacade->writeGameWindow("Unable to run lich.");
    }
    QThread::sleep(1);
}

bool Lich::isRunning() {
    QReadLocker locker(&lock);
    return lich_proc->state() == QProcess::Running;
}

void Lich::killLich() {
    lich_proc->kill();
    lich_proc->waitForFinished(1000);
}

void Lich::sendMessage(QByteArray message) {
    if(lich_proc->isOpen() && lich_proc->isWritable()) {
        lich_proc->write(message);
    }
}

void Lich::displayOutputMsg() {
    lich_proc->setReadChannel(QProcess::StandardOutput);
    QByteArray msg = lich_proc->readAll();
    qDebug() << msg.data();
}

void Lich::displayErrorMsg() {
    lich_proc->setReadChannel(QProcess::StandardError);
    QByteArray msg = lich_proc->readAll();
    windowFacade->writeGameWindow(msg);
    qDebug() << "Error: " << (msg.data());
}

void Lich::start() {
}

void Lich::finish(int code) {
    windowFacade->writeGameWindow(QString("<br/>--- Lich process exited with code " +
                                          QString::number(code) + "<br/>").toLocal8Bit());
    lich_proc->closeWriteChannel();
}

void Lich::handleError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) {
        windowFacade->writeGameWindow("The lich process failed to start. "
                                      "Either the Ruby installation is missing, "
                                      "or you may have insufficient permissions to "
                                      "invoke Ruby installation.");
    }
}

Lich::~Lich() {
    this->killLich();
    delete lich_proc;
}
