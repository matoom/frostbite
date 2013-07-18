#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : QPlainTextEdit(parent) {
    mainWindow = (MainWindow*)parent;       
    windowManager = mainWindow->getWindowManager();
    settings = new GeneralSettings();
    snapshot = new Snapshot(this);

    this->setObjectName("Main");

    this->loadSettings();   
    this->buildContextMenu();

    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);
    this->setUndoRedoEnabled(false);
    this->document()->setMaximumBlockCount(GAME_WINDOW_LIMIT);

    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(enableCopy(bool)));

    /* workaround for bottom margin */
    setViewportMargins(0, 0, 0, -6);
}

void GameWindow::showEvent(QShowEvent* event) {
    QPlainTextEdit::showEvent(event);
}

void GameWindow::loadSettings() {
    QFont font = settings->gameWindowFont();
    this->setFont(font);

    QColor fontColor = settings->gameWindowFontColor();
    QPalette p = this->viewport()->palette();
    p.setColor(QPalette::Text, fontColor);

    this->setPalette(p);
}

QColor GameWindow::getBgColor() {
    return mainWindow->getBackgroundColor();
}

QColor GameWindow::getTextColor() {
    return viewport()->palette().color(QPalette::Text);
}

QTextDocument* GameWindow::getDocument() {
    return this->document();
}

QString GameWindow::getObjectName() {
    return this->objectName();
}

QPlainTextEdit* GameWindow::getMainWindow() {
    return this;
}

void GameWindow::buildContextMenu() {
    menu = new QMenu(this);

    copyAct = new QAction(tr("&Copy\t"), this);
    menu->addAction(copyAct);
    copyAct->setEnabled(false);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySelected()));

    selectAct = new QAction(tr("&Select all\t"), this);
    menu->addAction(selectAct);
    connect(selectAct, SIGNAL(triggered()), this, SLOT(selectAll()));

    menu->addSeparator();

    saveAct = new QAction(tr("&Save\t"), this);
    menu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveAsHtml()));

    menu->addSeparator();

    clearAct = new QAction(tr("&Clear\t"), this);
    menu->addAction(clearAct);
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));
}

void GameWindow::saveAsHtml() {
    snapshot->save();
}

void GameWindow::contextMenuEvent(QContextMenuEvent *event) {
    menu->exec(event->globalPos());
}

void GameWindow::resizeEvent(QResizeEvent *event) {
    windowManager->paintNavigationDisplay();
    QPlainTextEdit::resizeEvent(event);
}

void GameWindow::enableCopy(bool enabled) {
    copyAct->setEnabled(enabled);
}

void GameWindow::copySelected() {
    this->copy();

    QTextCursor textCursor = this->textCursor();
    textCursor.clearSelection();
    this->setTextCursor(textCursor);
}

GameWindow::~GameWindow() {        
    delete copyAct;
    delete selectAct;
    delete clearAct;
    delete saveAct;
    delete menu;
    delete snapshot;
    delete settings;
}
