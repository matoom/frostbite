#include "hyperlinkservice.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "globaldefines.h"


HyperlinkService::HyperlinkService(QObject *parent) : QObject(parent) {
    QDesktopServices::setUrlHandler(FROSTBITE_SCHEMA, this, "handleUrl");
}

void HyperlinkService::handleUrl(const QUrl &url) {
    if (url.host() == "action") {
        QString action = url.toDisplayString(QUrl::RemoveScheme).remove("//action/");
        handleActionCommand(action);
    }
}

void HyperlinkService::handleActionCommand(const QString &action) {
    emit actionCommand(action);
}

HyperlinkService::~HyperlinkService() {  
}


