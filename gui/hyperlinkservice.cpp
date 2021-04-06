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
        handleActionCommand(QUrl::fromPercentEncoding(action.toLatin1()));
    }
}

void HyperlinkService::handleActionCommand(const QString &action) {
    QStringList commands = action.split(QLatin1Char(';'), Qt::SkipEmptyParts);
    if (commands.size() == 1) {
        emit actionCommand(commands.at(0));
    } else {
        emit actionCommands(commands);
    }
}

HyperlinkService::~HyperlinkService() {  
}


