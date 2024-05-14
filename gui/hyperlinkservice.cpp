#include "hyperlinkservice.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "globaldefines.h"
#include "mainwindow.h"
#include "hyperlinkutils.h"

HyperlinkService::HyperlinkService(QObject *parent) : QObject(parent) {
    QDesktopServices::setUrlHandler(FROSTBITE_SCHEMA, this, "handleUrl");

    mainWindow = (MainWindow*)parent;

    connect(this, SIGNAL(actionCommand(const QString&)),
            mainWindow, SLOT(actionCommand(const QString&)));
    connect(this, SIGNAL(actionCommands(const QStringList&)),
            mainWindow, SLOT(actionCommands(const QStringList&)));
}

void HyperlinkService::handleUrl(const QUrl &url) {
    if (url.host() == "a") {
        QString action = url.toDisplayString(QUrl::RemoveScheme).remove("//a/");
        handleActionCommand(QByteArray::fromBase64(action.toLocal8Bit()));
    }

    //QDesktopServices::unsetUrlHandler(FROSTBITE_SCHEMA);
}

void HyperlinkService::handleActionCommand(const QString &action) {
    QStringList commands = action.split(QLatin1Char(';'), QString::SkipEmptyParts);
    if (commands.size() == 1) {
        emit actionCommand(commands.at(0));
    } else {
        emit actionCommands(commands);
    }
}

HyperlinkService::~HyperlinkService() {  
}


