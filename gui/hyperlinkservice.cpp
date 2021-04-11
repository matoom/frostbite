#include "hyperlinkservice.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "globaldefines.h"
#include "mainwindow.h"

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
}

void HyperlinkService::handleActionCommand(const QString &action) {
    QStringList commands = action.split(QLatin1Char(';'), QString::SkipEmptyParts);
    if (commands.size() == 1) {
        emit actionCommand(commands.at(0));
    } else {
        emit actionCommands(commands);
    }
}

void HyperlinkService::addLink(QString &text, const QString &pattern, const QString &command) {
    QRegExp rx;
    rx.setPattern(pattern % "(?=[^>]*(<|$))");

    int index = rx.indexIn(text);
    if(index != -1) {
        int count = rx.captureCount();
        if(count == 0) {
            int pos = index;
            while ((pos = rx.indexIn(text, pos)) != -1) {
                pos += HyperlinkService::createLink(text, command, pos, rx.cap(0));
            }
        } else {
            int inserted = 0;
            for(int i = 1; i < count + 1; i++) {
                inserted += HyperlinkService::createLink(text, command, rx.pos(i) + inserted, rx.cap(i));
            }
        }
    }
}

int HyperlinkService::createLink(QString &text, const QString &command, int indexStart, QString match) {
    QString startTag = "<a href=\"" + HyperlinkService::createCommand(match, command) + "\">";
    QString endTag = "</a>";

    int startTagLength = startTag.length();
    int indexEnd = indexStart + startTagLength + match.length();

    text.insert(indexStart, startTag);
    text.insert(indexEnd, endTag);

    return startTagLength + endTag.length();
}

QString HyperlinkService::createCommand(QString text, QString command) {
    command.replace(QRegExp("\\$1"), text);
    return FROSTBITE_SCHEMA + QString("://a/") + command.toLocal8Bit().toBase64();
}

HyperlinkService::~HyperlinkService() {  
}


