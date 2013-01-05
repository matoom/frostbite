#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // register types
    qRegisterMetaType<DirectionsList>("DirectionsList");

    // application settings
    this->appSetup();
    // build client content
    this->loadClient();
    // load client settings
    this->initSettings();

    ui->statusBar->hide();
}

void MainWindow::appSetup() {
    // set cleanlooks as base style
    QApplication::setStyle("cleanlooks");

    // does not open on correct screen in 4.8
    // https://bugreports.qt-project.org/browse/QTBUG-21371

    // load client window state
    settings = ClientSettings::Instance();
    restoreGeometry(settings->getParameter("MainWindow/geometry", NULL).toByteArray());
    restoreState(settings->getParameter("MainWindow/state", NULL).toByteArray());

    // load general settings
    generalSettings = new GeneralSettings();
}

void MainWindow::toggleFullScreen() {
    setWindowState(Qt::WindowFullScreen);
}

void MainWindow::toggleMaximized() {
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::updateProfileSettings() {
    // highlights
    cm->updateSettings();
    // general highlights/window coloring
    windowManager->reloadSettings();
    // macros
    cmdLine->updateMacroSettings();
    // quick buttons
    tbm->updateQuickButtonSettings();
    // dialogs
    menuHandler->updateDialogSettings();
}

void MainWindow::initSettings() {
    QColor color = generalSettings->gameWindowBackground();

    /* set centralWidget background color to black */
    /* background for gamewindow is transparent on navi collage */
    QPalette palette = ui->centralWidget->palette();
    palette.setColor(QPalette::Window, color);
    ui->centralWidget->setPalette(palette);

    /* set focus to command line at startup */
    cmdLine->setFocus();
}

void MainWindow::setBackgroundColor(QColor color) {
    QPalette palette = ui->centralWidget->palette();
    palette.setColor(QPalette::Window, color);
    ui->centralWidget->setPalette(palette);
}

void MainWindow::loadClient() {
    windowManager = new WindowManager(this);
    windowManager->loadWindows();

    tbm = new ToolbarManager(this);
    tbm->loadToolbar();

    timerBar = new TimerBar(this);
    timerBar->loadProgressbar();

    cmdLine = new CommandLine(this);
    ui->mainLayout->addWidget(cmdLine);

    scriptService = new ScriptService(this);

    cm = new ConnectionManager(this);

    menuHandler = new MenuHandler(this);
    connect(ui->menuBar, SIGNAL(triggered(QAction*)), menuHandler, SLOT(menuTriggered(QAction*)));
    connect(ui->menuBar, SIGNAL(hovered(QAction*)), menuHandler, SLOT(menuHovered(QAction*)));
}

WindowManager* MainWindow::getWindowManager() {
    return windowManager;
}

ToolbarManager* MainWindow::getToolbarManager() {
    return tbm;
}

ConnectionManager* MainWindow::getConnectionManager() {
    return cm;
}

CommandLine* MainWindow::getCommandLine() {
    return cmdLine;
}

ScriptService* MainWindow::getScriptService() {
    return scriptService;
}

TimerBar* MainWindow::getTimerBar() {
    QReadLocker locker(&lock);
    return this->timerBar;
}

void MainWindow::addWidgetMainLayout(QWidget* widget) {
    ui->mainLayout->addWidget(widget);
}

void MainWindow::addDockWidgetMainWindow(Qt::DockWidgetArea area, QDockWidget *dock) {
    QAction *action = dock->toggleViewAction();
    action->setText(action->text() + " window");
    ui->menuWindow->addAction(action);

    if(settings->hasValue("MainWindow/state")) {
        restoreDockWidget(dock);
    } else {
        addDockWidget(area, dock);
    }
}

void MainWindow::addToolbarWidget(QWidget *widget) {
    ui->mainToolBar->addWidget(widget);
}

void MainWindow::addToolbarSpacer(int w) {
    QWidget* spacer = new QWidget(ui->mainToolBar);
    spacer->setFixedWidth(w);

    ui->mainToolBar->addWidget(spacer);
}

void MainWindow::addToolbarAction(QAction *action) {
    ui->mainToolBar->addAction(action);
}

void MainWindow::addToolbarSeparator() {
    ui->mainToolBar->addSeparator();
}

void MainWindow::insertProfilesMenu(QMenu* menu) {
    ui->actionLoad_profile->setMenu(menu);
}

void MainWindow::setToolbarAllowedAreas(Qt::ToolBarAreas areas) {
    ui->mainToolBar->setAllowedAreas(areas);
}

void MainWindow::setMainTitle(QString roomName) {
    setWindowTitle("The Frostbite Client" + roomName);
}

void MainWindow::connectEnabled(bool enabled) {
    ui->actionConnect->setEnabled(enabled);
}

void MainWindow::closeEvent(QCloseEvent *event){
    /* save client window state */
    settings->setParameter("MainWindow/state", saveState());
    settings->setParameter("MainWindow/geometry", saveGeometry());

    /* terminate if script running at exit */
    scriptService->terminateScript();
}

MainWindow::~MainWindow() {
    delete cm;
    delete settings;
    delete generalSettings;
    delete ui;
    delete tbm;
    delete windowManager;
    delete cmdLine;
    delete menuHandler;
    delete timerBar;
}
