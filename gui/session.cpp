#include "session.h"

#include "mainwindow.h"
#include "tcpclient.h"
#include "xml/xmlparserthread.h"
#include "lich/lich.h"
#include "gamedatacontainer.h"

// classes we connect events from xmlparser to
#include "vitalsbar.h"
#include "toolbar/toolbar.h"
#include "commandline.h"
#include "roundtimedisplay.h"
#include "windowfacade.h"
#include "window/conversationswindow.h"
#include "window/roomwindow.h"
#include "window/expwindow.h"
#include "window/deathswindow.h"
#include "window/thoughtswindow.h"
#include "window/arrivalswindow.h"
#include "window/familiarwindow.h"
#include "window/spellwindow.h"
#include "window/atmosphericswindow.h"
#include "window/groupwindow.h"
#include "window/combatwindow.h"
#include "scriptservice.h"

Session::Session(MainWindow* parent, bool debug)
    : QObject(parent), mainWindow(static_cast<MainWindow*>(parent)) {
    lich = new Lich(mainWindow);
    tcpClient = new TcpClient(this, lich, debug);
    xmlParser = new XmlParserThread(mainWindow, GameDataContainer::Instance());

    bindParserAndClient();
    bindClientStatus();
    bindVitalsBar();
    bindToolBar();
    bindCommandLine();
    bindWindowFacade();
    bindWindows();
    bindScriptService();
    bindMainWindow();

    if (!xmlParser->isRunning()) {
        xmlParser->start();
    }

    tcpClient->init();
}

void Session::bindParserAndClient() {
    // Connect XML parses and TCP Client
    connect(tcpClient, SIGNAL(addToQueue(QByteArray)), xmlParser, SLOT(addData(QByteArray)));
    connect(tcpClient, SIGNAL(diconnected()), xmlParser, SLOT(flushStream()));
    connect(xmlParser, SIGNAL(writeSettings()), tcpClient, SLOT(writeSettings()));
    connect(xmlParser, SIGNAL(writeModeSettings()), tcpClient, SLOT(writeModeSettings()));
    connect(xmlParser, SIGNAL(writeDefaultSettings(QString)), tcpClient,
            SLOT(writeDefaultSettings(QString)));
    connect(xmlParser, SIGNAL(gameModeIsCmgr(bool)), tcpClient, SLOT(setGameModeCmgr(bool)));
}

void Session::bindClientStatus() {
    // Connect TCP Client events to main window events, to
    // report status
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

void Session::bindVitalsBar() {
    // Connect events from xmlparser to vitals bar
    connect(xmlParser, SIGNAL(updateVitals(QString, QString)), mainWindow->getVitalsBar(),
            SLOT(updateVitals(QString, QString)));
}

void Session::bindToolBar() {
    // Connect events from xmlparser to toolbar
    connect(xmlParser, SIGNAL(updateVitals(QString, QString)), mainWindow->getToolbar(),
            SLOT(updateVitals(QString, QString)));
    connect(xmlParser, SIGNAL(updateStatus(QString, QString)), mainWindow->getToolbar(),
            SLOT(updateStatus(QString, QString)));
    connect(xmlParser, SIGNAL(updateWieldLeft(QString)), mainWindow->getToolbar(),
            SLOT(updateWieldLeft(QString)));
    connect(xmlParser, SIGNAL(updateWieldRight(QString)), mainWindow->getToolbar(),
            SLOT(updateWieldRight(QString)));
    connect(xmlParser, SIGNAL(updateSpell(QString)), mainWindow->getToolbar(),
            SLOT(updateSpell(QString)));
    connect(xmlParser, SIGNAL(updateActiveSpells(QStringList)), mainWindow->getToolbar(),
            SLOT(updateActiveSpells(QStringList)));
    connect(xmlParser, SIGNAL(clearActiveSpells()), mainWindow->getToolbar(),
            SLOT(clearActiveSpells()));
}

void Session::bindCommandLine() {
    // Connect events from xmlparser to command line UI
    connect(xmlParser, SIGNAL(setTimer(int)), mainWindow->getCommandLine()->getRoundtimeDisplay(),
            SLOT(setTimer(int)));
    connect(xmlParser, SIGNAL(setCastTimer(int)),
            mainWindow->getCommandLine()->getRoundtimeDisplay(), SLOT(setCastTimer(int)));
}

void Session::bindWindowFacade() {
    // Connect events from xmlparser to WindowFacade
    connect(xmlParser, SIGNAL(updateNavigationDisplay(DirectionsList)),
            mainWindow->getWindowFacade(), SLOT(updateNavigationDisplay(DirectionsList)));
    connect(xmlParser, SIGNAL(updateMapWindow(QString)), mainWindow->getWindowFacade(),
            SLOT(updateMapWindow(QString)));
    connect(xmlParser, SIGNAL(writeText(QByteArray, bool)), mainWindow->getWindowFacade(),
            SLOT(writeGameText(QByteArray, bool)));
    connect(xmlParser, SIGNAL(registerStreamWindow(QString, QString)),
            mainWindow->getWindowFacade(), SLOT(registerStreamWindow(QString, QString)));
    connect(xmlParser, SIGNAL(writeStreamWindow(QString, QString)), mainWindow->getWindowFacade(),
            SLOT(writeStreamWindow(QString, QString)));
    connect(xmlParser, SIGNAL(clearStreamWindow(QString)), mainWindow->getWindowFacade(),
            SLOT(clearStreamWindow(QString)));
}

void Session::bindWindows() {
    // Connect events from xmlparser to stand-alone windows
    connect(xmlParser, SIGNAL(updateConversationsWindow(QString)),
            mainWindow->getWindowFacade()->getConversationsWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateRoomWindow()), mainWindow->getWindowFacade()->getRoomWindow(),
            SLOT(write()));
    connect(xmlParser, SIGNAL(updateRoomWindowTitle(QString)),
            mainWindow->getWindowFacade()->getRoomWindow(), SLOT(setTitle(QString)));
    connect(xmlParser, SIGNAL(updateExpWindow(QString, QString)),
            mainWindow->getWindowFacade()->getExpWindow(), SLOT(write(QString, QString)));
    connect(xmlParser, SIGNAL(updateDeathsWindow(QString)),
            mainWindow->getWindowFacade()->getDeathsWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateThoughtsWindow(QString)),
            mainWindow->getWindowFacade()->getThoughtsWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateArrivalsWindow(QString)),
            mainWindow->getWindowFacade()->getArrivalsWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateFamiliarWindow(QString)),
            mainWindow->getWindowFacade()->getFamiliarWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateSpellWindow(QString)),
            mainWindow->getWindowFacade()->getSpellWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateAtmosphericsWindow(QString)),
            mainWindow->getWindowFacade()->getAtmosphericsWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateGroupWindow(QString)),
            mainWindow->getWindowFacade()->getGroupWindow(), SLOT(write(QString)));
    connect(xmlParser, SIGNAL(updateCombatWindow(QString)),
            mainWindow->getWindowFacade()->getCombatWindow(), SLOT(write(QString)));
}

void Session::bindScriptService() {
    // Connect events from xmlparser to script service
    connect(xmlParser, SIGNAL(writeScriptMessage(QByteArray)), mainWindow->getScriptService(),
            SLOT(writeScriptText(QByteArray)));
}

void Session::bindMainWindow() {
    // Connect events from xmlparser to MainWindow.
    connect(xmlParser, SIGNAL(setMainTitle(QString)), mainWindow, SLOT(setMainTitle(QString)));
}
