#include "dictionaryservice.h"
#include "dictionarysettings.h"

#include <QMessageBox>

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
    if (word.size() > 3)
        emit translationFinished("translation\n");
    else {
        emit translationFailed("Not Found\n");
    }
}


void DictionaryService::processErrorOccurred(QProcess::ProcessError error) {
    emit translationFailed("Unable to execute dictionary\n");
}

void DictionaryService::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode != 0) {
        emit translationFailed("Word not found\n");
    } else {
        QString translation = process->readAllStandardOutput();
        emit translationFinished(translation);
    }
}
