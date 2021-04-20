#include "debuglogger.h"

DebugLogger::DebugLogger(QObject*) {
}

void DebugLogger::addText(QString text) {
    Parent::addData(text);
}

void DebugLogger::onProcess(const QString& text) {
    logger()->info(text);
}
