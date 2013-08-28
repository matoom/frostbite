#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    genericWindowFactory = new GenericWindowFactory(parent);
    navigationDisplay = new NavigationDisplay(parent);
    gameDataContainer = GameDataContainer::Instance();
    clientSettings = ClientSettings::Instance();
    settings = new HighlightSettings();
    generalSettings = new GeneralSettings();

    rxRemoveTags.setPattern("<[^>]*>");

    writePrompt = true;
}

void WindowManager::reloadSettings() {
    this->reloadHighlighterSettings();

    settings->init();
    this->updateWindowStyle();

    generalSettings->init();
    this->updateWindowColors();    
}

void WindowManager::reloadHighlighterSettings() {
    emit updateGameWindowSettings();
    emit updateRoomSettings();
    emit updateExpSettings();
    emit updateArrivalsSettings();
    emit updateThoughtsSettings();
    emit updateDeathsSettings();
    emit updateConversationsSettings();
    emit updateFamiliarSettings();
}

QPlainTextEdit* WindowManager::getGameWindow() {
    return this->gameWindow;
}

void WindowManager::updateWindowColors() {
    this->setGameWindowFontColor(generalSettings->gameWindowFontColor());
    this->setGameWindowFont(generalSettings->gameWindowFont());
    mainWindow->setBackgroundColor(generalSettings->gameWindowBackground());

    this->setDockBackground(generalSettings->dockWindowBackground());
    this->setDockFont(generalSettings->dockWindowFont());
    this->setDockFontColor(generalSettings->dockWindowFontColor());
}

QString WindowManager::textColor(QString name, QString defaultValue) {
    return settings->getSingleParameter(
        "GeneralHighlight/" + name + "/color", defaultValue).value<QColor>().name();
}

void WindowManager::setGameWindowFont(QFont font) {
    gameWindow->setFont(font);
}

void WindowManager::setGameWindowFontColor(QColor color) {
    QPalette p = gameWindow->viewport()->palette();
    p.setColor(QPalette::Text, color);

    gameWindow->viewport()->setPalette(p);    
}

void WindowManager::setDockFontColor(QColor fontColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {        
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Text, fontColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowManager::setDockBackground(QColor backgroundColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Base, backgroundColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowManager::setDockFont(QFont font) {    
    foreach(QDockWidget* dock, dockWindows) {
        ((QPlainTextEdit*)dock->widget())->setFont(font);
    }
}

void WindowManager::updateWindowStyle() {
    style = ".speech {color: " + textColor(SPEECH, SPEECH_COLOR_HEX) + ";}\n"
            ".whisper {color: " + textColor(WHISPER, WHISPER_COLOR_HEX) + ";}\n"
            ".bonus {color: " + textColor(BONUS, BOOST_COLOR_HEX) + ";}\n"
            ".penalty {color: " + textColor(PENALTY, PENALTY_COLOR_HEX) + ";}\n"
            ".thinking {color: " + textColor(THINKING, THINKING_COLOR_HEX) + ";}\n"
            ".room-name {color: " + textColor(ROOM_NAME, ROOM_NAME_COLOR_HEX) + ";}\n"
            ".echo {color: " + textColor(ECHO, ECHO_COLOR_HEX) + ";}\n"
            ".script {color: " + textColor(SCRIPT, SCRIPT_COLOR_HEX) + ";}\n"
            ".bold {color: " + textColor(GAME_MESSAGE, GAME_MESSAGE_COLOR_HEX) + ";}\n"
            ".damage {color: " + textColor(DAMAGE, DAMAGE_COLOR_HEX) + ";}\n"
            "span {white-space:pre-wrap;}";

    foreach(QDockWidget* dock, dockWindows) {
        ((QPlainTextEdit*)dock->widget())->document()->setDefaultStyleSheet(style);
    }

    ((GameWindow*)this->gameWindow)->document()->setDefaultStyleSheet(style);
}

QString WindowManager::getStyle() {
    return style;
}

void WindowManager::loadWindows() {                
    gameWindow = (QPlainTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    roomWindow = genericWindowFactory->createWindow(DOCK_TITLE_ROOM);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, roomWindow);
    dockWindows << roomWindow;

    //qDebug() << ((QPlainTextEdit*)roomWindow->widget())->toPlainText();

    //https://bugreports.qt-project.org/browse/QTBUG-16252

    arrivalsWindow = genericWindowFactory->createWindow(DOCK_TITLE_ARRIVALS);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, arrivalsWindow);
    dockWindows << arrivalsWindow;
    connect(arrivalsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(arrivalsVisibility(bool)));

    deathsWindow = genericWindowFactory->createWindow(DOCK_TITLE_DEATHS);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, deathsWindow);
    dockWindows << deathsWindow;
    connect(deathsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(deathsVisibility(bool)));

    thoughtsWindow = genericWindowFactory->createWindow(DOCK_TITLE_THOUGHTS);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, thoughtsWindow);
    dockWindows << thoughtsWindow;
    connect(thoughtsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(thoughtsVisibility(bool)));

    expWindow = genericWindowFactory->createWindow(DOCK_TITLE_EXP);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, expWindow);
    dockWindows << expWindow;

    conversationsWindow = genericWindowFactory->createWindow(DOCK_TITLE_CONVERSATIONS);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, conversationsWindow);
    dockWindows << conversationsWindow;
    connect(conversationsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(conversationsVisibility(bool)));

    familiarWindow = genericWindowFactory->createWindow(DOCK_TITLE_FAMILIAR);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, familiarWindow);
    dockWindows << familiarWindow;
    connect(familiarWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(familiarVisibility(bool)));

    this->initWindowHighlighters();
    this->initLoggers();

    if(!clientSettings->hasValue("MainWindow/state")) {
        mainWindow->tabifyDockWidget(thoughtsWindow, arrivalsWindow);
        mainWindow->tabifyDockWidget(arrivalsWindow, deathsWindow);
        mainWindow->tabifyDockWidget(deathsWindow, familiarWindow);
        mainWindow->tabifyDockWidget(roomWindow, conversationsWindow);        
    }

    this->updateWindowStyle();    
}

void WindowManager::initWindowHighlighters() {
    gameWindowHighlighter = new HighlighterThread(mainWindow, (GameWindow*)gameWindow);
    connect(this, SIGNAL(updateGameWindowSettings()), gameWindowHighlighter, SLOT(updateSettings()));
    highlighters << gameWindowHighlighter;

    ((GenericWindow*)roomWindow->widget())->setAppend(false);
    roomHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)roomWindow->widget());
    connect(this, SIGNAL(updateRoomSettings()), roomHighlighter, SLOT(updateSettings()));
    highlighters << roomHighlighter;

    arrivalsHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)arrivalsWindow->widget());
    connect(this, SIGNAL(updateArrivalsSettings()), arrivalsHighlighter, SLOT(updateSettings()));
    highlighters << arrivalsHighlighter;

    deathsHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)deathsWindow->widget());
    connect(this, SIGNAL(updateDeathsSettings()), deathsHighlighter, SLOT(updateSettings()));
    highlighters << deathsHighlighter;

    thoughtsHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)thoughtsWindow->widget());
    connect(this, SIGNAL(updateThoughtsSettings()), thoughtsHighlighter, SLOT(updateSettings()));
    highlighters << thoughtsHighlighter;

    ((GenericWindow*)expWindow->widget())->setAppend(false);
    expHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)expWindow->widget());
    connect(this, SIGNAL(updateExpSettings()), expHighlighter, SLOT(updateSettings()));
    highlighters << expHighlighter;

    conversationsHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)conversationsWindow->widget());
    connect(this, SIGNAL(updateConversationsSettings()), conversationsHighlighter, SLOT(updateSettings()));
    highlighters << conversationsHighlighter;

    familiarHighlighter = new HighlighterThread(mainWindow, (GenericWindow*)familiarWindow->widget());
    connect(this, SIGNAL(updateFamiliarSettings()), familiarHighlighter, SLOT(updateSettings()));
    highlighters << familiarHighlighter;
}

void WindowManager::initLoggers() {
    mainLogger = new MainLogger();
    thoughtsLogger = new ThoughtsLogger();
    conversationsLogger = new ConversationsLogger();
    deathsLogger = new DeathsLogger();
    arrivalsLogger = new ArrivalsLogger();
}

void WindowManager::thoughtsVisibility(bool visibility) {
    thoughtsWindow->setWindowTitle(DOCK_TITLE_THOUGHTS);
    thoughtsVisible = visibility;
    //https://bugreports.qt-project.org/browse/QTBUG-840
}

void WindowManager::deathsVisibility(bool visibility) {
    deathsWindow->setWindowTitle(DOCK_TITLE_DEATHS);
    deathsVisible = visibility;
}

void WindowManager::arrivalsVisibility(bool visibility) {
    arrivalsWindow->setWindowTitle(DOCK_TITLE_ARRIVALS);
    arrivalsVisible = visibility;
}

void WindowManager::conversationsVisibility(bool visibility) {
    conversationsWindow->setWindowTitle(DOCK_TITLE_CONVERSATIONS);
    conversationsVisible = visibility;
}

void WindowManager::familiarVisibility(bool visibility) {
    familiarWindow->setWindowTitle(DOCK_TITLE_FAMILIAR);
    familiarVisible = visibility;
}

void WindowManager::setVisibilityIndicator(QDockWidget* widget, bool visible, QString title) {
    if(visible) {
        widget->setWindowTitle(title);
    } else {
        widget->setWindowTitle(title + " *");
    }
}

QDockWidget* WindowManager::getRoomWindow() {
    return this->roomWindow;
}

QDockWidget* WindowManager::getArrivalsWindow() {
    return this->arrivalsWindow;
}

QDockWidget* WindowManager::getThoughtsWindow() {
    return this->thoughtsWindow;
}

QDockWidget* WindowManager::getExpWindow() {
    return this->expWindow;
}

QDockWidget* WindowManager::getDeathsWindow() {
    return this->deathsWindow;
}

QDockWidget* WindowManager::getConversationsWindow() {
    return this->conversationsWindow;
}

QDockWidget* WindowManager::getFamiliarWindow() {
    return this->familiarWindow;
}

void WindowManager::copyDock() {
    foreach(QDockWidget* dock, dockWindows) {
        QTextCursor cursor = ((QPlainTextEdit*)dock->widget())->textCursor();
        if(cursor.hasSelection()) {
            ((QPlainTextEdit*)dock->widget())->copy();
            break;
        }
    }
}

void WindowManager::updateNavigationDisplay(DirectionsList directions) {
    navigationDisplay->updateState(directions);
    this->paintNavigationDisplay();
}

void WindowManager::scriptRunning(bool state) {
    navigationDisplay->setAutoPilot(state);
    this->paintNavigationDisplay();
}

/* paints a full screen image to background */
void WindowManager::paintNavigationDisplay() {    
    QPixmap image = navigationDisplay->paint();

    QPixmap collage(gameWindow->width(), gameWindow->height());
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    int x = gameWindow->width() - image.width() - 25;
    int y = gameWindow->height() - image.height() - 5;

    painter.drawPixmap(QRectF(x, y, image.width(), image.height()), image,
                       QRectF(0, 0, image.width(), image.height()));

    QPalette palette = gameWindow->viewport()->palette();
    palette.setBrush(QPalette::Base, QBrush(collage));
    gameWindow->viewport()->setPalette(palette);
}

void WindowManager::updateExpWindow() {
    QHash<QString, QString> exp = gameDataContainer->getExp();

    QString expString = "";
    foreach (QString value, exp) {
        expString += value + "\n";
    }

    expHighlighter->addText(expString);

    if(!expHighlighter->isRunning()) {
        expHighlighter->start();
    }
}

void WindowManager::updateConversationsWindow(QString conversationText) {
    setVisibilityIndicator(conversationsWindow, conversationsVisible, DOCK_TITLE_CONVERSATIONS);

    conversationsHighlighter->addText(conversationText.trimmed());

    if(!conversationsHighlighter->isRunning()) {
        conversationsHighlighter->start();
    }
    this->logConversationsText(conversationText);
}

void WindowManager::logConversationsText(QString conversationText) {
    if(clientSettings->getParameter("Logging/conversations", false).toBool()) {
        conversationsLogger->addText(conversationText.trimmed());

        if(!conversationsLogger->isRunning()) {
            conversationsLogger->start();
        }
    }
}

void WindowManager::updateDeathsWindow(QString deathText) {
    setVisibilityIndicator(deathsWindow, deathsVisible, DOCK_TITLE_DEATHS);

    deathsHighlighter->addText(deathText.trimmed());

    if(!deathsHighlighter->isRunning()) {
        deathsHighlighter->start();
    }
    this->logDeathsText(deathText);
}

void WindowManager::logDeathsText(QString deathsText) {
    if(clientSettings->getParameter("Logging/deaths", false).toBool()) {
        deathsLogger->addText(deathsText.trimmed());

        if(!deathsLogger->isRunning()) {
            deathsLogger->start();
        }
    }
}

void WindowManager::updateThoughtsWindow(QString thoughtText) {
    setVisibilityIndicator(thoughtsWindow, thoughtsVisible, DOCK_TITLE_THOUGHTS);

    thoughtsHighlighter->addText(thoughtText.trimmed());

    if(!thoughtsHighlighter->isRunning()) {
        thoughtsHighlighter->start();
    }
    this->logThoughtsText(thoughtText);
}

void WindowManager::logThoughtsText(QString thoughtText) {
    if(clientSettings->getParameter("Logging/thoughts", false).toBool()) {
        thoughtsLogger->addText(thoughtText.trimmed());

        if(!thoughtsLogger->isRunning()) {
            thoughtsLogger->start();
        }
    }
}

void WindowManager::updateArrivalsWindow(QString arrivalText) {
    setVisibilityIndicator(arrivalsWindow, arrivalsVisible, DOCK_TITLE_ARRIVALS);

    arrivalsHighlighter->addText(arrivalText.trimmed());

    if(!arrivalsHighlighter->isRunning()) {
        arrivalsHighlighter->start();
    }
    this->logArrivalsText(arrivalText);
}

void WindowManager::logArrivalsText(QString arrivalText) {
    if(clientSettings->getParameter("Logging/arrivals", false).toBool()) {
        arrivalsLogger->addText(arrivalText);

        if(!arrivalsLogger->isRunning()) {
            arrivalsLogger->start();
        }
    }
}

void WindowManager::updateRoomWindow() {
    QString roomText = "";

    QString desc = gameDataContainer->getRoomDesc();
    roomText += desc.isEmpty() ? "" : desc + "\n";

    QString objs = gameDataContainer->getRoomObjs();
    roomText += objs.isEmpty() ? "" : objs + "\n";

    QString players = gameDataContainer->getRoomPlayers();
    roomText += players.isEmpty() ? "" : players + "\n";

    QString exits = gameDataContainer->getRoomExits();
    roomText += exits.isEmpty() ? "" : exits + "\n";

    roomHighlighter->addText(roomText);

    if(!roomHighlighter->isRunning()) {
        roomHighlighter->start();
    }
}

void WindowManager::updateFamiliarWindow(QString familiarText) {
    setVisibilityIndicator(familiarWindow, familiarVisible, DOCK_TITLE_FAMILIAR);

    familiarHighlighter->addText(familiarText.trimmed());

    if(!familiarHighlighter->isRunning()) {
        familiarHighlighter->start();
    }
}

void WindowManager::updateRoomWindowTitle(QString title) {
    roomWindow->setWindowTitle("Room " + title);
}

void WindowManager::writeGameText(QByteArray text, bool prompt) {
    if(prompt && writePrompt) {
        gameWindowHighlighter->addText(text);
        mainWindow->getScriptService()->writeScriptText(text);
        this->logGameText(text, 'p');
        writePrompt = false;
    } else if(!prompt) {
        gameWindowHighlighter->addText(text);
        mainWindow->getScriptService()->writeScriptText(text);
        this->logGameText(text);
        writePrompt = true;
    }

    if(!gameWindowHighlighter->isRunning()) {
        gameWindowHighlighter->start();
    }
}

void WindowManager::writeGameWindow(QByteArray text) {
    gameWindowHighlighter->addText(text);

    if(!gameWindowHighlighter->isRunning()) {
        gameWindowHighlighter->start();
    }
    this->logGameText(text);
}

void WindowManager::logGameText(QByteArray text, char type) {
    if(clientSettings->getParameter("Logging/main", false).toBool()) {
        mainLogger->addText(text, type);

        if(!mainLogger->isRunning()) {
            mainLogger->start();
        }
    }
}

WindowManager::~WindowManager() {
    delete genericWindowFactory;
    delete gameWindow;
    delete navigationDisplay;    

    foreach(QDockWidget* dock, dockWindows) {
        delete dock;
    }

    foreach(HighlighterThread* highlighter, highlighters) {
        delete highlighter;
    }

    delete mainLogger;
    delete thoughtsLogger;
    delete conversationsLogger;
    delete arrivalsLogger;
    delete deathsLogger;

    delete settings;
    delete generalSettings;
}
