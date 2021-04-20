#include "thoughtslogger.h"
#include "textutils.h"

ThoughtsLogger::ThoughtsLogger(QObject*) {
    rxRemoveTags.setPattern("<[^>]*>");
}

void ThoughtsLogger::addText(QString text) {
    Parent::addData(text);
}

void ThoughtsLogger::log(QString logText) {
    TextUtils::htmlToPlain(logText);
    logger()->info(logText.remove(rxRemoveTags));
}
