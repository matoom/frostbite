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
}

void MainWindow::appSetup() {
    /* load client window state */
    settings = ClientSettings::Instance();
    restoreState(settings->getParameter("MainWindow", NULL).toByteArray());

    /* start maximized */
    setWindowState(Qt::WindowMaximized);

    /* set cleanlooks as base style */
    QApplication::setStyle("cleanlooks");
}

void MainWindow::initSettings() {
    /* set centralWidget background color to black */
    QPalette palette = ui->centralWidget->palette();
    palette.setColor(QPalette::Window, Qt::black);
    ui->centralWidget->setPalette(palette);

    /* set gameWindow background to transparent to show compass */
    palette = wm->getGameWindow()->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    wm->getGameWindow()->viewport()->setPalette(palette);

    /* set focus to command line at startup */
    cmdLine->setFocus();
}

void MainWindow::loadClient() {
    wm = new WindowManager(this);
    wm->loadWindows();

    tbm = new ToolbarManager(this);
    tbm->loadToolbar();

    QProgressBar *timer = new QProgressBar();
    timer->setValue(100);
    timer->setMaximumHeight(2);
    timer->setTextVisible(false);
    ui->mainLayout->addWidget(timer);

    cmdLine = new CommandLine(this);
    ui->mainLayout->addWidget(cmdLine);

    cm = new ConnectionManager(this);

    scriptService = new ScriptService(this);

    menuHandler = new MenuHandler(this);
    connect(ui->menuBar, SIGNAL(triggered(QAction*)), menuHandler, SLOT(menuTriggered(QAction*)));
}

WindowManager* MainWindow::getWindowManager() {
    return wm;
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

void MainWindow::addWidgetMainLayout(QWidget* widget) {
    ui->mainLayout->addWidget(widget);
}

void MainWindow::addDockWidgetMainWindow(Qt::DockWidgetArea area, QDockWidget *dock) {
    QAction *action = dock->toggleViewAction();
    action->setText(action->text() + " window");
    ui->menuWindow->addAction(action);

    addDockWidget(area, dock);
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

void MainWindow::setMainTitle(QString roomName) {
    setWindowTitle("The Frostbite Client" + roomName);
}

void MainWindow::closeEvent(QCloseEvent *event){
    /* save client window state */
    settings->setParameter("MainWindow", saveState());
}

MainWindow::~MainWindow() {
    delete cm;
    delete settings;
    delete ui;
    delete tbm;
    delete wm;
    delete cmdLine;
    delete menuHandler;
}
