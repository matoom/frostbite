#include "deathslogger.h"

DeathsLogger::DeathsLogger(QObject*) {
}

void DeathsLogger::addText(QString text) {
    Parent::addData(text);
}

void DeathsLogger::onProcess(const QString& text) {
    logger()->info(text);
}
