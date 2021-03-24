#include "dictionaryservice.h"
#include "dictionarysettings.h"

DictionaryService::DictionaryService(QObject* parent) :
    QObject(parent),
    settings(DictionarySettings::getInstance()),
    process(new QProcess(this)) {
    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)),
            this, SLOT(processErrorOccurred(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
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
        if (exitCode == 0 || exitCode == 21) {
            // 21 is the error code for dict meaning
            // "Approximate matches found"
            QString translation = process->readAllStandardOutput();
            emit translationFinished(translation);            
        } else {
            emitError("Word not found");
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
