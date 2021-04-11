#include "dictionaryservice.h"
#include "dictionarysettings.h"

#include "mainlogger.h"
#include "mainwindow.h"
#include "windowfacade.h"
#include "window/dictionarywindow.h"

DictionaryService::DictionaryService(QObject* parent) :
    QObject(parent),
    settings(DictionarySettings::getInstance()),
    process(new QProcess(this)) {

    mainWindow = (MainWindow*)parent;

    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)),
            this, SLOT(processErrorOccurred(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));

    connect(this, SIGNAL(translationFinished(QString)),
            mainWindow->getWindowFacade()->getDictionaryWindow(), SLOT(write(QString)));
    connect(this, SIGNAL(translationFailed(QString)),
            mainWindow->getWindowFacade()->getDictionaryWindow(), SLOT(write(QString)));
}

DictionaryService::~DictionaryService() {}

void DictionaryService::translate(const QString &word) {
    if (process->state() == QProcess::NotRunning) {
        QStringList args;
        args = settings->getDictArguments().split(" ");
        args << word;
        process->start(settings->getDictCommand(), args);
    } else {
        emitError("Dictionary process is already running");
    }
}


void DictionaryService::processErrorOccurred(QProcess::ProcessError error) {
    QString reason;
    switch (error) {
    case QProcess::FailedToStart:
        reason = "File not found, resource error";
        break;
    case QProcess::Crashed:
        reason = "Process crashed";
        break;
    case QProcess::Timedout:
        reason = "Process timed out";
        break;
    case QProcess::ReadError:
        reason = "Read error";
        break;
    case QProcess::WriteError:
        reason = "Write error";
        break;
    default:
        reason = "Unknown error";
        break;
    }
    emitError(reason);
}

void DictionaryService::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    switch (exitStatus) {
    case QProcess::NormalExit:
        switch (exitCode) {
        case 0: {
            QString translation = process->readAllStandardOutput();
            emit translationFinished(translation);                        
        } break;
        case 21: { // dict error code "Approximate matches found"
            QString translation = process->readAllStandardError();
            emit translationFinished(translation);            
        } break;
        default:
            emitError("Word not found");
            break;
        }
        break;
    default:
        emitError("Process crashed");
        break;
    }
}


void DictionaryService::emitError(const QString& reason) {
    emit translationFailed("(Error: " + reason + ")\n");
}
