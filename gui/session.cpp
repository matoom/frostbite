#include "session.h"

#include "mainwindow.h"
#include "tcpclient.h"
#include "xml/xmlparserthread.h"

Session::Session(MainWindow* parent, TcpClient* client, XmlParserThread* parser) : QObject(parent), tcpClient(client), xmlParser(parser) {
    connect(tcpClient, SIGNAL(addToQueue(QByteArray)), xmlParser, SLOT(addData(QByteArray)));
    connect(tcpClient, SIGNAL(diconnected()), xmlParser, SLOT(flushStream()));
    connect(xmlParser, SIGNAL(writeSettings()), tcpClient, SLOT(writeSettings()));
    connect(xmlParser, SIGNAL(writeModeSettings()), tcpClient, SLOT(writeModeSettings()));
    connect(xmlParser, SIGNAL(writeDefaultSettings(QString)), tcpClient, SLOT(writeDefaultSettings(QString)));
    connect(xmlParser, SIGNAL(gameModeIsCmgr(bool)), tcpClient, SLOT(setGameModeCmgr(bool)));

    
    if(!xmlParser->isRunning()) {
        xmlParser->start();
    }

    tcpClient->init();
}
