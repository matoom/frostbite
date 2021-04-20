#include "authlogger.h"

AuthLogger::AuthLogger(QObject*) {
}

void AuthLogger::addText(QString text) {
    Parent::addData(text);
}

void AuthLogger::onProcess(const QString &text) {
  logger()->info(text);
}

