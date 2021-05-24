#include "dictionaryservice.h"
#include "dictionarysettings.h"

#include "mainlogger.h"
#include "mainwindow.h"
#include "windowfacade.h"
#include "window/dictionarywindow.h"

DictionaryService::DictionaryService(QObject* parent)
    : Parent(parent), settings(DictionarySettings::getInstance()) {

    mainWindow = (MainWindow*)parent;
    updateConnections();
    start();
}

DictionaryService::~DictionaryService() {}

void DictionaryService::translate(const QString &word) {
    Parent::addData(word);
}

void DictionaryService::onProcess(const QString& word) {
    QStringList args;
    QProcess process;
    args = settings->getDictArguments().split(" ");
    args << word;
    process.start(settings->getDictCommand(), args);
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit) {
        emitError("Process crashed");
    } else {
        switch (process.exitCode()) {
        case 0: {
            QString translation = process.readAllStandardOutput();
            qDebug() << "Translation finished: " << word;
            emit translationFinished(word, translation);
        } break;
        case 21: { // dict error code "Approximate matches found"
            QString translation = process.readAllStandardError();
            emit translationFinished(word, translation);
        } break;
        default:
            emitError("Word not found");
            break;
        }
    }
}

void DictionaryService::emitError(const QString& reason) {
    emit translationFailed("(Error: " + reason + ")\n");
}

void DictionaryService::updateConnections() {
    // remove all connections
    disconnect(SIGNAL(translationFinished(QString, QString)));
    disconnect(SIGNAL(translationFailed(QString)));

    switch (settings->getDictOutputType()) {
    case DictionarySettings::OutputType::Window:
        connect(this, SIGNAL(translationFinished(QString, QString)),
                mainWindow->getWindowFacade()->getDictionaryWindow(),
                SLOT(write(QString, QString)));
        connect(this, SIGNAL(translationFailed(QString)),
                mainWindow->getWindowFacade()->getDictionaryWindow(), SLOT(write(QString)));
        break;
    case DictionarySettings::OutputType::Tooltip:
        connect(this, SIGNAL(translationFinished(QString, QString)),
                mainWindow->getWindowFacade()->getGameWindow(),
                SLOT(translationFinished(QString, QString)));
        connect(this, SIGNAL(translationFailed(QString)),
                mainWindow->getWindowFacade()->getGameWindow(), SLOT(translationFailed(QString)));
        break;
    case DictionarySettings::OutputType::Disabled:
        // Do nothing. We have already disconnected all signals
        break;
    }
}
