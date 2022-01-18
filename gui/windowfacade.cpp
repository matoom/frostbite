#include "windowfacade.h"
#include "mainwindow.h"
#include "gamewindow.h"
#include "genericwindowfactory.h"
#include "gridwindowfactory.h"
#include "gridwindow.h"
#include "gamedatacontainer.h"
#include "compass.h"
#include "compass/compassview.h"
#include "defaultvalues.h"
#include "text/highlight/highlighter.h"
#include "text/highlight/highlightsettings.h"
#include "windowwriterthread.h"
#include "gridwriterthread.h"
#include "mainlogger.h"
#include "thoughtslogger.h"
#include "conversationslogger.h"
#include "deathslogger.h"
#include "arrivalslogger.h"
#include "maps/mapfacade.h"

#include "window/roomwindow.h"
#include "window/arrivalswindow.h"
#include "window/deathswindow.h"
#include "window/thoughtswindow.h"
#include "window/expwindow.h"
#include "window/conversationswindow.h"
#include "window/familiarwindow.h"
#include "window/spellwindow.h"
#include "window/atmosphericswindow.h"
#include "window/groupwindow.h"
#include "window/combatwindow.h"
#include "window/dictionarywindow.h"

#include "clientsettings.h"
#include "generalsettings.h"
#include "scriptservice.h"
#include "genericwindow.h"
#include "windowwriterthread.h"

QStringList WindowFacade::staticWindows = QStringList() << "inv" << "familiar" << "thoughts"
    << "logons" << "death" << "assess" << "conversation" << "whispers" << "talk" << "experience"
    << "group" << "atmospherics" << "ooc" << "room" << "percWindow" << "chatter" << "dictionary";

WindowFacade::WindowFacade(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;    
    genericWindowFactory = new GenericWindowFactory(parent);
    compass = new Compass(parent);
    gameDataContainer = GameDataContainer::Instance();
    clientSettings = ClientSettings::getInstance();
    settings = HighlightSettings::getInstance();
    generalSettings = GeneralSettings::getInstance();
    mainLogger = new MainLogger(this);

    rxRemoveTags.setPattern("<[^>]*>");

    writePrompt = true;

    connect(mainWindow, SIGNAL(profileChanged()), this, SLOT(reloadSettings()));
    connect(this, SIGNAL(updateWindowSettings()), mainLogger, SLOT(updateSettings()));    
    connect(mainWindow, SIGNAL(writeMainWindow(QByteArray)), this, SLOT(writeGameWindow(QByteArray)));
}

void WindowFacade::reloadSettings() {
    this->reloadWindowSettings();

    settings = HighlightSettings::getInstance();
    generalSettings = GeneralSettings::getInstance();

    this->updateWindowStyle();
    this->updateWindowColors();
}

void WindowFacade::reloadWindowSettings() {
    emit updateWindowSettings();
}


QPlainTextEdit* WindowFacade::getGameWindow() {
    return this->gameWindow;
}

void WindowFacade::updateWindowColors() {
    this->setGameWindowFontColor(generalSettings->gameWindowFontColor());
    this->setGameWindowFont(generalSettings->gameWindowFont());
    this->setGameWindowBackground(generalSettings->gameWindowBackground());

    this->setDockBackground(generalSettings->dockWindowBackground());
    this->setDockFont(generalSettings->dockWindowFont());
    this->setDockFontColor(generalSettings->dockWindowFontColor());
}

QString WindowFacade::textColor(QString name, QString defaultValue) {
    return settings->getSingleParameter("GeneralHighlight/" + name + "/color", defaultValue)
            .value<QColor>().name();
}

QString WindowFacade::bgColor(QString name) {
    QColor color = settings->getSingleParameter("GeneralHighlight/" + name + "/bgColor", QColor()).value<QColor>();
    return color.isValid() ? color.name() : "none";
}

void WindowFacade::setGameWindowFont(QFont font) {
    gameWindow->setFont(font);
}

void WindowFacade::setGameWindowFontColor(QColor color) {
    QPalette p = gameWindow->viewport()->palette();
    p.setColor(QPalette::Text, color);
    gameWindow->viewport()->setPalette(p);    
}

void WindowFacade::setGameWindowBackground(QColor color) {
    QPalette p = gameWindow->viewport()->palette();
    p.setColor(QPalette::Base, color);
    gameWindow->viewport()->setPalette(p);
}

void WindowFacade::setDockFontColor(QColor fontColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {
        if(qobject_cast<QPlainTextEdit*>(dock->widget()) != NULL) {
            p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
            p.setColor(QPalette::Text, fontColor);
            ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
        } else if(qobject_cast<QTableWidget*>(dock->widget()) != NULL) {
            QTableWidget* tableWidget = (QTableWidget*)dock->widget();
            for(int i = 0; i < tableWidget->rowCount(); i++) {
                QLabel* widget = (QLabel*)tableWidget->cellWidget(i, 0);
                QPalette p = widget->palette();
                p.setColor(QPalette::Text, fontColor);
                widget->setPalette(p);
            }
        }
    }

    foreach(QDockWidget* dock, streamWindows) {
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Text, fontColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowFacade::setDockBackground(QColor backgroundColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {
        if(qobject_cast<QPlainTextEdit*>(dock->widget()) != NULL) {
            p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
            p.setColor(QPalette::Base, backgroundColor);
            ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
        } else if(qobject_cast<QTableWidget*>(dock->widget()) != NULL) {
            QTableWidget* tableWidget = (QTableWidget*)dock->widget();

            QPalette p;
            p = tableWidget->viewport()->palette();
            p.setColor(QPalette::Base, backgroundColor);
            tableWidget->viewport()->setPalette(p);

            for(int i = 0; i < tableWidget->rowCount(); i++) {
                QWidget* widget = tableWidget->cellWidget(i, 0);
                QLabel* label = ((QLabel*)widget);

                p = label->palette();
                p.setColor(QPalette::Base, backgroundColor);
                ((QLabel*)widget)->setPalette(p);
            }
        }
    }

    foreach(QDockWidget* dock, streamWindows) {
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Base, backgroundColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }

    mapFacade->updateMapColors(backgroundColor);
}

void WindowFacade::setDockFont(QFont font) {
    foreach(QDockWidget* dock, dockWindows) {
        QVariant windowFont = dock->widget()->property(WINDOW_FONT_ID);
        if(qobject_cast<QPlainTextEdit*>(dock->widget()) != NULL) {
            ((QPlainTextEdit*)dock->widget())->setFont(windowFont.isNull() ? font : windowFont.value<QFont>());
        } else if(qobject_cast<QTableWidget*>(dock->widget()) != NULL) {
            if(windowFont.isNull()) {
                QTableWidget* tableWidget = (QTableWidget*)dock->widget();
                for(int i = 0; i < tableWidget->rowCount(); i++) {
                    QWidget* widget = tableWidget->cellWidget(i, 0);
                    if(widget != NULL) {
                        ((QLabel*)widget)->setFont(windowFont.isNull() ? font : windowFont.value<QFont>());
                    }
                }
            }
        }
    }

    foreach(QDockWidget* dock, streamWindows) {                
        QVariant windowFont = dock->widget()->property(WINDOW_FONT_ID);
        ((QPlainTextEdit*)dock->widget())->setFont(windowFont.isNull() ? font : windowFont.value<QFont>());
    }
}

void WindowFacade::updateWindowStyle() {
    style = ".speech {color: " + textColor(SPEECH, SPEECH_COLOR_HEX) + ";background:" + bgColor(SPEECH) + ";}\n"
            ".whisper {color: " + textColor(WHISPER, WHISPER_COLOR_HEX) + ";background:" + bgColor(WHISPER) + ";}\n"
            ".bonus {color: " + textColor(BONUS, BOOST_COLOR_HEX) + ";background:" + bgColor(BONUS) + ";}\n"
            ".penalty {color: " + textColor(PENALTY, PENALTY_COLOR_HEX) + ";background:" + bgColor(PENALTY) + ";}\n"
            ".thinking {color: " + textColor(THINKING, THINKING_COLOR_HEX) + ";background:" + bgColor(THINKING) + ";}\n"
            ".room-name {color: " + textColor(ROOM_NAME, ROOM_NAME_COLOR_HEX) + ";background:" + bgColor(ROOM_NAME) + ";}\n"
            ".echo {color: " + textColor(ECHO, ECHO_COLOR_HEX) + ";background:" + bgColor(ECHO) + ";}\n"
            ".script {color: " + textColor(SCRIPT, SCRIPT_COLOR_HEX) + ";background:" + bgColor(SCRIPT) + ";}\n"
            ".bold {color: " + textColor(GAME_MESSAGE, GAME_MESSAGE_COLOR_HEX) + ";background:" + bgColor(GAME_MESSAGE) + ";}\n"
            ".damage {color: " + textColor(DAMAGE, DAMAGE_COLOR_HEX) + ";background:" + bgColor(DAMAGE) + ";}\n"
            "span {white-space:pre-wrap;}";

    foreach(QDockWidget* dock, dockWindows) {
        if(qobject_cast<QPlainTextEdit*>(dock->widget()) != NULL) {
            ((QPlainTextEdit*)dock->widget())->document()->setDefaultStyleSheet(style);
        }
    }
    ((GameWindow*)this->gameWindow)->document()->setDefaultStyleSheet(style);
}

QString WindowFacade::getStyle() {
    return style;
}

void WindowFacade::loadWindows() {
    gameWindow = (QPlainTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    mainWriter = new WindowWriterThread(mainWindow, (GameWindow*)gameWindow);
    connect(this, SIGNAL(updateWindowSettings()), mainWriter, SLOT(updateSettings()));

    roomWindow = new RoomWindow(mainWindow);
    dockWindows << roomWindow->getDockWidget();

    mapFacade = new MapFacade(mainWindow);

    conversationsWindow = new ConversationsWindow(mainWindow);
    dockWindows << conversationsWindow->getDockWidget();

    arrivalsWindow = new ArrivalsWindow(mainWindow);
    dockWindows << arrivalsWindow->getDockWidget();

    deathsWindow = new DeathsWindow(mainWindow);
    dockWindows << deathsWindow->getDockWidget();

    thoughtsWindow = new ThoughtsWindow(mainWindow);
    dockWindows << thoughtsWindow->getDockWidget();

    familiarWindow = new FamiliarWindow(mainWindow);
    dockWindows << familiarWindow->getDockWidget();

    spellWindow = new SpellWindow(mainWindow);
    dockWindows << spellWindow->getDockWidget();

    atmosphericsWindow = new AtmosphericsWindow(mainWindow);
    dockWindows << atmosphericsWindow->getDockWidget();

    groupWindow = new GroupWindow(mainWindow);
    dockWindows << groupWindow->getDockWidget();

    expWindow = new ExpWindow(mainWindow);
    dockWindows << expWindow->getDockWidget();

    combatWindow = new CombatWindow(mainWindow);
    dockWindows << combatWindow->getDockWidget();

    dictionaryWindow = new DictionaryWindow(mainWindow);
    dockWindows << dictionaryWindow->getDockWidget();

    compassView = new CompassView(mainWindow);
    compassView->paint(compass);

    if(!clientSettings->hasValue("MainWindow/state")) {
        mainWindow->tabifyDockWidget(mapFacade->getMapWindow(), roomWindow->getDockWidget());
        mainWindow->tabifyDockWidget(roomWindow->getDockWidget(), conversationsWindow->getDockWidget());
        mainWindow->tabifyDockWidget(conversationsWindow->getDockWidget(), groupWindow->getDockWidget());

        mainWindow->tabifyDockWidget(thoughtsWindow->getDockWidget(), arrivalsWindow->getDockWidget());
        mainWindow->tabifyDockWidget(arrivalsWindow->getDockWidget(), deathsWindow->getDockWidget());
        mainWindow->tabifyDockWidget(deathsWindow->getDockWidget(), familiarWindow->getDockWidget());
        mainWindow->tabifyDockWidget(familiarWindow->getDockWidget(), spellWindow->getDockWidget());        
        mainWindow->tabifyDockWidget(spellWindow->getDockWidget(), atmosphericsWindow->getDockWidget());
        mainWindow->tabifyDockWidget(atmosphericsWindow->getDockWidget(), combatWindow->getDockWidget());
        mainWindow->tabifyDockWidget(combatWindow->getDockWidget(), dictionaryWindow->getDockWidget());

        groupWindow->getDockWidget()->close();
        combatWindow->getDockWidget()->close();
        atmosphericsWindow->getDockWidget()->close();
        familiarWindow->getDockWidget()->close();
        dictionaryWindow->getDockWidget()->close();
    }

    this->updateWindowStyle();

    if(clientSettings->getParameter("Window/lock", false).toBool()) {
        this->lockWindows();
    }
}


QStringList WindowFacade::getWindowNames() {
    QStringList names;
    names << gameWindow->objectName();
    for(QDockWidget* dock : dockWindows) {
        names << dock->objectName();
    }
    return names;
}

QList<QDockWidget*> WindowFacade::getDockWindows() {
    return this->dockWindows;
}

RoomWindow* WindowFacade::getRoomWindow() {
    return this->roomWindow;
}

ArrivalsWindow* WindowFacade::getArrivalsWindow() {
    return this->arrivalsWindow;
}

ThoughtsWindow* WindowFacade::getThoughtsWindow() {
    return this->thoughtsWindow;
}

ExpWindow* WindowFacade::getExpWindow() {
    return this->expWindow;
}

DeathsWindow* WindowFacade::getDeathsWindow() {
    return this->deathsWindow;
}

ConversationsWindow* WindowFacade::getConversationsWindow() {
    return this->conversationsWindow;
}

FamiliarWindow* WindowFacade::getFamiliarWindow() {
    return this->familiarWindow;
}

SpellWindow* WindowFacade::getSpellWindow() {
    return this->spellWindow;
}

AtmosphericsWindow* WindowFacade::getAtmosphericsWindow() {
    return this->atmosphericsWindow;
}

GroupWindow* WindowFacade::getGroupWindow() {
    return this->groupWindow;
}

CombatWindow* WindowFacade::getCombatWindow() {
    return this->combatWindow;
}

DictionaryWindow* WindowFacade::getDictionaryWindow() {
    return this->dictionaryWindow;
}


MapFacade* WindowFacade::getMapFacade() {
    return this->mapFacade;
}

CompassView* WindowFacade::getCompassView() {
    return this->compassView;
}

void WindowFacade::copyDock() {
    foreach(QDockWidget* dock, dockWindows) {
        if(qobject_cast<QPlainTextEdit*>(dock->widget()) != NULL) {
            QTextCursor cursor = ((QPlainTextEdit*)dock->widget())->textCursor();
            if(cursor.hasSelection()) {
                ((QPlainTextEdit*)dock->widget())->copy();
                break;
            }
        }
    }
}

void WindowFacade::updateNavigationDisplay(DirectionsList directions) {
    compass->updateState(directions);
    this->paintCompass();
}

void WindowFacade::scriptRunning(bool state) {
    compass->setAutoPilot(state);
    this->paintCompass();
}

void WindowFacade::paintCompass() {
    compassView->paint(compass);
}

void WindowFacade::gameWindowResizeEvent(GameWindow* gameWindow) {
    compassView->gameWindowResizeEvent(gameWindow);
}

void WindowFacade::updateMapWindow(QString hash) {
    mapFacade->updateMapWindow(hash);
}

void WindowFacade::writeGameText(QByteArray text, bool prompt) {
    if(prompt && writePrompt) {
        mainWindow->getScriptService()->writeScriptText(text);
        mainWriter->addText(text);
        this->logGameText(text, MainLogger::PROMPT);
        writePrompt = false;
    } else if(!prompt) {
        mainWindow->getScriptService()->writeScriptText(text);
        mainWriter->addText(text);
        this->logGameText(text);
        writePrompt = true;
    }    

    if(!mainWriter->isRunning()) {
        mainWriter->start();
    }
}

void WindowFacade::writeGameWindow(QByteArray text) {
    mainWriter->addText(text);

    if(!mainWriter->isRunning()) {
        mainWriter->start();
    }
    this->logGameText(text);
}

void WindowFacade::logGameText(QByteArray text, char type) {
    if(clientSettings->getParameter("Logging/main", false).toBool()) {
        mainLogger->addText(text, type);

        if(!mainLogger->isRunning()) {
            mainLogger->start();
        }
    }
}

void WindowFacade::registerStreamWindow(QString id, QString title) {
    // Check if window has been registered (or it is a static window)
    if(streamWindows.contains(id) || staticWindows.contains(id)) return;
    
    QDockWidget* streamWindow = genericWindowFactory->createWindow(title.toLatin1().constData());
    ((GenericWindow*)streamWindow->widget())->setStream(true);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, streamWindow);
    streamWindows.insert(id, streamWindow);

    WindowWriterThread* streamWriter = new WindowWriterThread(mainWindow, (GenericWindow*)streamWindow->widget());
    connect(this, SIGNAL(updateWindowSettings()), streamWriter, SLOT(updateSettings()));
    streamWriters.insert(id, streamWriter);
}

void WindowFacade::removeStreamWindow(QString id) {
    if(!streamWindows.contains(id)) return;

    WindowWriterThread* writer = streamWriters.value(id);
    streamWriters.remove(id);
    delete writer;


    QDockWidget* window = streamWindows.value(id);
    mainWindow->removeDockWidgetMainWindow(window);
    delete window;
    streamWindows.remove(id);
}

QList<QString> WindowFacade::getStreamWindowNames() {
    return streamWindows.keys();
}

void WindowFacade::writeStreamWindow(QString id, QString text) {
    WindowWriterThread* streamWriter = streamWriters.value(id);
    if(streamWriter == NULL) return;

    streamWriter->addText(text);
    if(!streamWriter->isRunning()) {
        streamWriter->start();
    }
}

void WindowFacade::clearStreamWindow(QString id) {
    // Do not clear static windows
    if (!staticWindows.contains(id)) {
        this->writeStreamWindow(id, "{clear}");
    }
}

void WindowFacade::lockWindows() {
    foreach(QDockWidget* dock, dockWindows) {
        dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    }
    mapFacade->getMapWindow()->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void WindowFacade::unlockWindows() {
    foreach(QDockWidget* dock, dockWindows) {
        dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    }
    mapFacade->getMapWindow()->setFeatures(QDockWidget::AllDockWidgetFeatures);
}

WindowFacade::~WindowFacade() {
    delete compass;
    foreach(WindowWriterThread* writer, streamWriters) {
        delete writer;
    }
    delete mainWriter;

    foreach(QDockWidget* dock, streamWindows) {
        delete dock;
    }

    delete mapFacade->getMapWindow();    
    delete compassView;
    delete gameWindow;
    delete mainLogger;
}
