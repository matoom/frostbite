#include "arrivalslogger.h"

ArrivalsLogger::ArrivalsLogger(QObject*) {
}

void ArrivalsLogger::addText(QString text) {
    Parent::addData(text);
}

void ArrivalsLogger::onProcess(const QString& text) {
    logger()->info(text);    
}

