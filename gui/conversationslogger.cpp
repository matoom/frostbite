#include "conversationslogger.h"
#include "textutils.h"

ConversationsLogger::ConversationsLogger(QObject*) {
    rxRemoveTags.setPattern("<[^>]*>");
}

void ConversationsLogger::addText(QString text) {
    Parent::addData(text);
}

void ConversationsLogger::log(QString logText) {
    TextUtils::htmlToPlain(logText);
    logger()->info(logText.remove(rxRemoveTags));
}
