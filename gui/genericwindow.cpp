#include "genericwindow.h"

GenericWindow::GenericWindow(QWidget *parent) : QPlainTextEdit(parent) {
    mainWindow = (MainWindow*)parent;
    settings = ClientSettings::Instance();

    this->buildContextMenu();
    this->loadSettings();

    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);
    this->document()->setMaximumBlockCount(1000);

    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(enableCopy(bool)));

    /* workaround for bottom margin */
    setViewportMargins(0, 0, 0, -DEFAULT_MAIN_FONT_SIZE);
}

void GenericWindow::loadSettings() {
    QFont font = settings->getParameter("GameWindow/font",
        QFont(DEFAULT_MAIN_FONT, DEFAULT_MAIN_FONT_SIZE)).value<QFont>();

    this->setFont(font);
}

void GenericWindow::buildContextMenu() {
    menu = new QMenu(this);

    copyAct = new QAction(tr("&Copy\t"), this);
    menu->addAction(copyAct);
    copyAct->setEnabled(false);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySelected()));

    menu->addSeparator();

    selectAct = new QAction(tr("&Select All\t"), this);
    menu->addAction(selectAct);
    connect(selectAct, SIGNAL(triggered()), this, SLOT(selectAll()));
}

void GenericWindow::contextMenuEvent(QContextMenuEvent* event) {
    menu->exec(event->globalPos());
}

void GenericWindow::enableCopy(bool enabled) {
    copyAct->setEnabled(enabled);
}

void GenericWindow::copySelected() {
    this->copy();

    QTextCursor textCursor = this->textCursor();
    textCursor.clearSelection();
    this->setTextCursor(textCursor);
}

GenericWindow::~GenericWindow() {
    delete copyAct;
    delete selectAct;
    delete menu;
}
