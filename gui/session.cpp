#include "session.h"

#include "mainwindow.h"
#include "tcpclient.h"
#include "xml/xmlparserthread.h"
#include "lich/lich.h"

Session::Session(MainWindow* parent, bool debug)
    : QObject(parent), mainWindow(static_cast<MainWindow*>(parent)) {
    lich = new Lich(mainWindow);
    tcpClient = new TcpClient(this, lich, debug);
    xmlParser = new XmlParserThread(mainWindow);
    
    setupParserAndClient();
    setupClientStatus();

    if (!xmlParser->isRunning()) {
        xmlParser->start();
    }

    tcpClient->init();
}

void Session::setupParserAndClient() {
    // Connect XML parses and TCP Client
    connect(tcpClient, SIGNAL(addToQueue(QByteArray)), xmlParser, SLOT(addData(QByteArray)));
    connect(tcpClient, SIGNAL(diconnected()), xmlParser, SLOT(flushStream()));
    connect(xmlParser, SIGNAL(writeSettings()), tcpClient, SLOT(writeSettings()));
    connect(xmlParser, SIGNAL(writeModeSettings()), tcpClient, SLOT(writeModeSettings()));
    connect(xmlParser, SIGNAL(writeDefaultSettings(QString)), tcpClient,
            SLOT(writeDefaultSettings(QString)));
    connect(xmlParser, SIGNAL(gameModeIsCmgr(bool)), tcpClient, SLOT(setGameModeCmgr(bool)));
}

void Session::setupClientStatus() {
    // Connect to TCP Client events and connect it to our events
    connect(tcpClient, SIGNAL(connectAvailable(bool)), mainWindow, SLOT(connectEnabled(bool)));
    connect(tcpClient, SIGNAL(connectStarted()), mainWindow, SLOT(connectStarted()));
    connect(tcpClient, SIGNAL(connectSucceeded()), mainWindow, SLOT(connectSucceeded()));
    connect(tcpClient, SIGNAL(connectFailed(QString)), mainWindow, SLOT(connectFailed(QString)));
}

TcpClient* Session::getTcpClient() {
    return tcpClient;
}

void Session::openConnection(QString host, QString port, QString key) {
    tcpClient->connectToHost(host, port, key);
}

void Session::openLocalConnection(QString port) {
    tcpClient->connectToLocalPort(port);
}
