#include "windowfacade.h"

WindowFacade::WindowFacade(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    genericWindowFactory = new GenericWindowFactory(parent);
    gridWindowFactory = new GridWindowFactory(parent);    
    navigationDisplay = new NavigationDisplay(parent);
    gameDataContainer = GameDataContainer::Instance();
    clientSettings = ClientSettings::Instance();
    settings = new HighlightSettings();
    generalSettings = new GeneralSettings();

    rxRemoveTags.setPattern("<[^>]*>");

    writePrompt = true;
}

void WindowFacade::reloadSettings() {
    this->reloadHighlighterSettings();

    settings->init();
    this->updateWindowStyle();

    generalSettings->init();
    this->updateWindowColors();    
}

void WindowFacade::reloadHighlighterSettings() {
    emit updateGameWindowSettings();
    emit updateRoomSettings();    
    emit updateArrivalsSettings();
    emit updateThoughtsSettings();
    emit updateDeathsSettings();
    emit updateConversationsSettings();
    emit updateFamiliarSettings();
    emit updateExpSettings();
    emit updateSpellSettings();
}

QPlainTextEdit* WindowFacade::getGameWindow() {
    return this->gameWindow;
}

void WindowFacade::updateWindowColors() {
    // game window
    this->setGameWindowFontColor(generalSettings->gameWindowFontColor());
    this->setGameWindowFont(generalSettings->gameWindowFont());
    mainWindow->setBackgroundColor(generalSettings->gameWindowBackground());

    // text window
    this->setTextDockBackground(generalSettings->dockWindowBackground());
    this->setTextDockFont(generalSettings->dockWindowFont());
    this->setTextDockFontColor(generalSettings->dockWindowFontColor());

    // grid window
    ((GridWindow*)expWindow->widget())->clearTracked();
    this->setGridDockBackground(generalSettings->dockWindowBackground());
    this->setGridDockFont(generalSettings->dockWindowFont());
    this->setGridDockFontColor(generalSettings->dockWindowFontColor());
}

QString WindowFacade::textColor(QString name, QString defaultValue) {
    return settings->getSingleParameter("GeneralHighlight/" + name + "/color", defaultValue)
            .value<QColor>().name();
}

void WindowFacade::setGameWindowFont(QFont font) {
    gameWindow->setFont(font);
}

void WindowFacade::setGameWindowFontColor(QColor color) {
    QPalette p = gameWindow->viewport()->palette();
    p.setColor(QPalette::Text, color);

    gameWindow->viewport()->setPalette(p);    
}

void WindowFacade::setTextDockFontColor(QColor fontColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {        
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Text, fontColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowFacade::setTextDockBackground(QColor backgroundColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Base, backgroundColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowFacade::setTextDockFont(QFont font) {
    foreach(QDockWidget* dock, dockWindows) {
        ((QPlainTextEdit*)dock->widget())->setFont(font);
    }
}

void WindowFacade::setGridDockFontColor(QColor fontColor) {
    foreach(QDockWidget* gridWindow, gridWindows) {
        QTableWidget* tableWidget = (QTableWidget*)gridWindow->widget();        
        for(int i = 0; i < tableWidget->rowCount(); i++) {
            QLabel* widget = (QLabel*)tableWidget->cellWidget(i, 0);
            QPalette p = widget->palette();
            p.setColor(QPalette::Text, fontColor);
            widget->setPalette(p);
        }
    }
}

void WindowFacade::setGridDockBackground(QColor backgroundColor) {
    foreach(QDockWidget* gridWindow, gridWindows) {
        QTableWidget* tableWidget = (QTableWidget*)gridWindow->widget();

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

void WindowFacade::setGridDockFont(QFont font) {
    foreach(QDockWidget* gridWindow, gridWindows) {
        QTableWidget* tableWidget = (QTableWidget*)gridWindow->widget();        
        for(int i = 0; i < tableWidget->rowCount(); i++) {
            QWidget* widget = tableWidget->cellWidget(i, 0);
            if(widget != NULL) {
                ((QLabel*)widget)->setFont(font);
            }
        }
    }
}

void WindowFacade::updateWindowStyle() {
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

QString WindowFacade::getStyle() {
    return style;
}

void WindowFacade::loadWindows() {
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

    expWindow = gridWindowFactory->createWindow(DOCK_TITLE_EXP);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, expWindow);
    gridWindows << expWindow;

    conversationsWindow = genericWindowFactory->createWindow(DOCK_TITLE_CONVERSATIONS);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, conversationsWindow);
    dockWindows << conversationsWindow;
    connect(conversationsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(conversationsVisibility(bool)));

    familiarWindow = genericWindowFactory->createWindow(DOCK_TITLE_FAMILIAR);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, familiarWindow);
    dockWindows << familiarWindow;
    connect(familiarWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(familiarVisibility(bool)));

    spellWindow = genericWindowFactory->createWindow(DOCK_TITLE_SPELL);
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, spellWindow);
    dockWindows << spellWindow;
    connect(spellWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(spellVisibility(bool)));

    mapFacade = new MapFacade(mainWindow);

    this->initWindowWriters();
    this->initLoggers();

    if(!clientSettings->hasValue("MainWindow/state")) {
        mainWindow->tabifyDockWidget(thoughtsWindow, arrivalsWindow);
        mainWindow->tabifyDockWidget(arrivalsWindow, deathsWindow);
        mainWindow->tabifyDockWidget(deathsWindow, familiarWindow);
        mainWindow->tabifyDockWidget(familiarWindow, spellWindow);
        mainWindow->tabifyDockWidget(roomWindow, conversationsWindow);
        mainWindow->tabifyDockWidget(conversationsWindow, mapFacade->getMapWindow());
    }

    this->updateWindowStyle();    
}

void WindowFacade::initWindowWriters() {
    mainWriter = new WindowWriterThread(mainWindow, (GameWindow*)gameWindow);
    connect(this, SIGNAL(updateGameWindowSettings()), mainWriter, SLOT(updateSettings()));
    writers << mainWriter;

    ((GenericWindow*)roomWindow->widget())->setAppend(false);
    roomWriter = new WindowWriterThread(mainWindow, (GenericWindow*)roomWindow->widget());
    connect(this, SIGNAL(updateRoomSettings()), roomWriter, SLOT(updateSettings()));
    writers << roomWriter;

    arrivalsWriter = new WindowWriterThread(mainWindow, (GenericWindow*)arrivalsWindow->widget());
    connect(this, SIGNAL(updateArrivalsSettings()), arrivalsWriter, SLOT(updateSettings()));
    writers << arrivalsWriter;

    deathsWriter = new WindowWriterThread(mainWindow, (GenericWindow*)deathsWindow->widget());
    connect(this, SIGNAL(updateDeathsSettings()), deathsWriter, SLOT(updateSettings()));
    writers << deathsWriter;

    thoughtsWriter = new WindowWriterThread(mainWindow, (GenericWindow*)thoughtsWindow->widget());
    connect(this, SIGNAL(updateThoughtsSettings()), thoughtsWriter, SLOT(updateSettings()));
    writers << thoughtsWriter;

    expWriter = new GridWriterThread(mainWindow);
    connect(expWriter, SIGNAL(writeGrid(GridItems)), this, SLOT(writeExpWindow(GridItems)));
    connect(this, SIGNAL(updateExpSettings()), expWriter, SLOT(updateSettings()));
    gridWriters << expWriter;

    conversationsWriter = new WindowWriterThread(mainWindow, (GenericWindow*)conversationsWindow->widget());
    connect(this, SIGNAL(updateConversationsSettings()), conversationsWriter, SLOT(updateSettings()));
    writers << conversationsWriter;

    familiarWriter = new WindowWriterThread(mainWindow, (GenericWindow*)familiarWindow->widget());
    connect(this, SIGNAL(updateFamiliarSettings()), familiarWriter, SLOT(updateSettings()));
    writers << familiarWriter;

    ((GenericWindow*)spellWindow->widget())->setAppend(false);
    spellWriter = new WindowWriterThread(mainWindow, (GenericWindow*)spellWindow->widget());
    connect(this, SIGNAL(updateSpellSettings()), spellWriter, SLOT(updateSettings()));
    writers << spellWriter;
}

void WindowFacade::initLoggers() {
    mainLogger = new MainLogger();
    thoughtsLogger = new ThoughtsLogger();
    conversationsLogger = new ConversationsLogger();
    deathsLogger = new DeathsLogger();
    arrivalsLogger = new ArrivalsLogger();
}

void WindowFacade::thoughtsVisibility(bool visibility) {
    thoughtsWindow->setWindowTitle(DOCK_TITLE_THOUGHTS);
    thoughtsVisible = visibility;
    //https://bugreports.qt-project.org/browse/QTBUG-840
}

void WindowFacade::deathsVisibility(bool visibility) {
    deathsWindow->setWindowTitle(DOCK_TITLE_DEATHS);
    deathsVisible = visibility;
}

void WindowFacade::arrivalsVisibility(bool visibility) {
    arrivalsWindow->setWindowTitle(DOCK_TITLE_ARRIVALS);
    arrivalsVisible = visibility;
}

void WindowFacade::conversationsVisibility(bool visibility) {
    conversationsWindow->setWindowTitle(DOCK_TITLE_CONVERSATIONS);
    conversationsVisible = visibility;
}

void WindowFacade::familiarVisibility(bool visibility) {
    familiarWindow->setWindowTitle(DOCK_TITLE_FAMILIAR);
    familiarVisible = visibility;
}

void WindowFacade::spellVisibility(bool visibility) {
    spellWindow->setWindowTitle(DOCK_TITLE_SPELL);
    spellVisible = visibility;
}

void WindowFacade::setVisibilityIndicator(QDockWidget* widget, bool visible, QString title) {
    if(visible) {
        widget->setWindowTitle(title);
    } else {
        widget->setWindowTitle(title + " *");
    }
}

QDockWidget* WindowFacade::getRoomWindow() {
    return this->roomWindow;
}

QDockWidget* WindowFacade::getArrivalsWindow() {
    return this->arrivalsWindow;
}

QDockWidget* WindowFacade::getThoughtsWindow() {
    return this->thoughtsWindow;
}

QDockWidget* WindowFacade::getExpWindow() {
    return this->expWindow;
}

QDockWidget* WindowFacade::getDeathsWindow() {
    return this->deathsWindow;
}

QDockWidget* WindowFacade::getConversationsWindow() {
    return this->conversationsWindow;
}

QDockWidget* WindowFacade::getFamiliarWindow() {
    return this->familiarWindow;
}

QDockWidget* WindowFacade::getSpellWindow() {
    return this->spellWindow;
}

MapFacade* WindowFacade::getMapFacade() {
    return this->mapFacade;
}

void WindowFacade::copyDock() {
    foreach(QDockWidget* dock, dockWindows) {
        QTextCursor cursor = ((QPlainTextEdit*)dock->widget())->textCursor();
        if(cursor.hasSelection()) {
            ((QPlainTextEdit*)dock->widget())->copy();
            break;
        }
    }
}

void WindowFacade::updateNavigationDisplay(DirectionsList directions) {
    navigationDisplay->updateState(directions);
    this->paintNavigationDisplay();
}

void WindowFacade::scriptRunning(bool state) {
    navigationDisplay->setAutoPilot(state);
    this->paintNavigationDisplay();
}

/* paints a full screen image to background */
void WindowFacade::paintNavigationDisplay() {    
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

void WindowFacade::writeExpWindow(GridItems items) {
    QTableWidget* table = ((QTableWidget*)expWindow->widget());
    GridWindow* window = ((GridWindow*)expWindow->widget());

    table->setRowCount(items.size());

    int i = 0;
    foreach(QString key, items.keys()) {        
        QString text = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body><span>";
        text += "<span>";
        if(gameDataContainer->isExpGained(key)) {
            text += "(+)";
        } else {
            text += "&nbsp;&nbsp;&nbsp;";
        }
        text += "</span>";
        text += items.value(key) + "</span></body></html>";

        QLabel* item = (QLabel*)table->cellWidget(i, 0);
        if(item != NULL) {
            item->setText(text);
            item->setObjectName(key);
            window->track(key, item);
        } else {
            QLabel* label = window->gridValueLabel(table, generalSettings, key);
            label->setText(text);
            table->setCellWidget(i++, 0, label);            
            window->track(key, label);
        }        
        i++;
    }
}

void WindowFacade::updateMapWindow(QString hash) {
    mapFacade->updateMapWindow(hash);
}

void WindowFacade::updateExpWindow(QString name, QString text) {
    expWriter->addItem(name, text);

    if(!expWriter->isRunning()) {
        expWriter->start();
    }
}

void WindowFacade::updateConversationsWindow(QString conversationText) {
    setVisibilityIndicator(conversationsWindow, conversationsVisible, DOCK_TITLE_CONVERSATIONS);

    conversationsWriter->addText(conversationText.trimmed());

    if(!conversationsWriter->isRunning()) {
        conversationsWriter->start();
    }
    this->logConversationsText(conversationText);
    mainWindow->getTray()->showMessage(DOCK_TITLE_CONVERSATIONS, conversationText.trimmed());
}

void WindowFacade::logConversationsText(QString conversationText) {
    if(clientSettings->getParameter("Logging/conversations", false).toBool()) {
        conversationsLogger->addText(conversationText.trimmed());

        if(!conversationsLogger->isRunning()) {
            conversationsLogger->start();
        }
    }
}

void WindowFacade::updateDeathsWindow(QString deathText) {
    setVisibilityIndicator(deathsWindow, deathsVisible, DOCK_TITLE_DEATHS);

    deathsWriter->addText(deathText.trimmed());

    if(!deathsWriter->isRunning()) {
        deathsWriter->start();
    }
    this->logDeathsText(deathText);
}

void WindowFacade::logDeathsText(QString deathsText) {
    if(clientSettings->getParameter("Logging/deaths", false).toBool()) {
        deathsLogger->addText(deathsText.trimmed());

        if(!deathsLogger->isRunning()) {
            deathsLogger->start();
        }
    }
}

void WindowFacade::updateThoughtsWindow(QString thoughtText) {
    setVisibilityIndicator(thoughtsWindow, thoughtsVisible, DOCK_TITLE_THOUGHTS);

    thoughtsWriter->addText(thoughtText.trimmed());

    if(!thoughtsWriter->isRunning()) {
        thoughtsWriter->start();
    }
    this->logThoughtsText(thoughtText);
}

void WindowFacade::logThoughtsText(QString thoughtText) {
    if(clientSettings->getParameter("Logging/thoughts", false).toBool()) {
        thoughtsLogger->addText(thoughtText.trimmed());

        if(!thoughtsLogger->isRunning()) {
            thoughtsLogger->start();
        }
    }
}

void WindowFacade::updateArrivalsWindow(QString arrivalText) {
    setVisibilityIndicator(arrivalsWindow, arrivalsVisible, DOCK_TITLE_ARRIVALS);

    arrivalsWriter->addText(arrivalText.trimmed());

    if(!arrivalsWriter->isRunning()) {
        arrivalsWriter->start();
    }
    this->logArrivalsText(arrivalText);
}

void WindowFacade::logArrivalsText(QString arrivalText) {
    if(clientSettings->getParameter("Logging/arrivals", false).toBool()) {
        arrivalsLogger->addText(arrivalText);

        if(!arrivalsLogger->isRunning()) {
            arrivalsLogger->start();
        }
    }
}

void WindowFacade::updateRoomWindow() {
    QString roomText = "";

    QString desc = gameDataContainer->getRoomDesc();
    roomText += desc.isEmpty() ? "" : desc + "\n";

    QString objs = gameDataContainer->getRoomObjs();
    roomText += objs.isEmpty() ? "" : objs + "\n";

    QString players = gameDataContainer->getRoomPlayers();
    roomText += players.isEmpty() ? "" : players + "\n";

    QString exits = gameDataContainer->getRoomExits();
    roomText += exits.isEmpty() ? "" : exits + "\n";

    roomWriter->addText(roomText);

    if(!roomWriter->isRunning()) {
        roomWriter->start();
    }
}

void WindowFacade::updateFamiliarWindow(QString familiarText) {
    setVisibilityIndicator(familiarWindow, familiarVisible, DOCK_TITLE_FAMILIAR);

    familiarWriter->addText(familiarText.trimmed());

    if(!familiarWriter->isRunning()) {
        familiarWriter->start();
    }
}

void WindowFacade::updateSpellWindow(QString spellText) {
    setVisibilityIndicator(spellWindow, spellVisible, DOCK_TITLE_SPELL);

    spellWriter->addText(spellText + "\n");
    if(!spellWriter->isRunning()) {
        spellWriter->start();
    }
}

void WindowFacade::updateRoomWindowTitle(QString title) {
    roomWindow->setWindowTitle("Room " + title);
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

WindowFacade::~WindowFacade() {
    delete genericWindowFactory;
    delete gameWindow;
    delete navigationDisplay;    

    foreach(QDockWidget* dock, dockWindows) {
        delete dock;
    }

    foreach(QDockWidget* dock, gridWindows) {
        delete dock;
    }

    foreach(WindowWriterThread* writer, writers) {
        // terminate threads at application exit.
        writer->terminate();
        delete writer;
    }

    foreach(GridWriterThread* writer, gridWriters) {
        writer->terminate();
        delete writer;
    }

    delete mainLogger;
    delete thoughtsLogger;
    delete conversationsLogger;
    delete arrivalsLogger;
    delete deathsLogger;

    delete settings;
    delete generalSettings;

    delete mapFacade->getMapWindow();
}
