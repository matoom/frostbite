#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    qRegisterMetaType<DirectionsList>("DirectionsList");

    /* application settings */
    this->appSetup();
    /* build client content */
    this->loadClient();
    /* load client settings */
    this->initSettings();

    ui->statusBar->hide();
}

void MainWindow::appSetup() {
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    /* load client window state */
    settings = ClientSettings::Instance();
    restoreState(settings->getParameter("MainWindow/state", NULL).toByteArray());
    restoreGeometry(settings->getParameter("MainWindow/geometry", NULL).toByteArray());    

    // does not open on correct screen in 4.8
    // https://bugreports.qt-project.org/browse/QTBUG-21371

    /* set cleanlooks as base style */
    QApplication::setStyle("cleanlooks");    
}

void MainWindow::toggleFullScreen() {
    setWindowState(Qt::WindowFullScreen);
}

void MainWindow::toggleMaximized() {
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::initSettings() {
    QColor color = settings->getParameter("GameWindow/background",
        DEFAULT_MAIN_BACKGROUND).value<QColor>();

    /* set centralWidget background color to black */
    /* background for gamewindow is transparent on navi collage */
    QPalette palette = ui->centralWidget->palette();
    palette.setColor(QPalette::Window, color);
    ui->centralWidget->setPalette(palette);

    /* set gameWindow background to transparent to show compass */
    /*palette = windowManager->getGameWindow()->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    windowManager->getGameWindow()->viewport()->setPalette(palette);*/

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
    delete ui;
    delete tbm;
    delete windowManager;
    delete cmdLine;
    delete menuHandler;
    delete timerBar;
}
