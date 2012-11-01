#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    /* application settings */
    this->appSetup();
    /* build client content */
    this->loadClient();
    /* load client settings */
    this->initSettings();

    ui->statusBar->hide();

    /* ping indicatior */

    /*QLabel *m_statusRight = new QLabel("200ms", this);
    m_statusRight->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    ui->statusBar->insertPermanentWidget(0, m_statusRight, 0);*/

    //ui->menuBar->find("actionConnect");
}

void MainWindow::appSetup() {
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    /* load client window state */
    settings = ClientSettings::Instance();
    restoreState(settings->getParameter("MainWindow/state", NULL).toByteArray());
    //restoreGeometry(settings->getParameter("MainWindow/geometry", NULL).toByteArray());

    /* start maximized */
    setWindowState(Qt::WindowMaximized);

    /* set cleanlooks as base style */
    QApplication::setStyle("cleanlooks");
}

void MainWindow::initSettings() {
    QColor color = settings->getParameter("GameWindow/background",
        DEFAULT_MAIN_BACKGROUND).value<QColor>();

    /* set centralWidget background color to black */
    QPalette palette = ui->centralWidget->palette();
    palette.setColor(QPalette::Window, color);
    ui->centralWidget->setPalette(palette);

    /* set gameWindow background to transparent to show compass */
    palette = windowManager->getGameWindow()->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    windowManager->getGameWindow()->viewport()->setPalette(palette);

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

    cm = new ConnectionManager(this);

    scriptService = new ScriptService(this);

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
    //settings->setParameter("MainWindow/geometry", saveGeometry());
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
