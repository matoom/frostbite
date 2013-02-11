#include "genericwindow.h"

GenericWindow::GenericWindow(QWidget *parent) : QPlainTextEdit(parent) {
    mainWindow = (MainWindow*)parent;
    settings = new GeneralSettings();

    this->buildContextMenu();
    this->loadSettings();

    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);
    this->document()->setMaximumBlockCount(1000);

    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(enableCopy(bool)));

    /* workaround for bottom margin */
    //setViewportMargins(0, 0, 0, -DEFAULT_MAIN_FONT_SIZE);
}

void GenericWindow::loadSettings() {
    QFont font = settings->dockWindowFont();
    this->setFont(font);
}

void GenericWindow::buildContextMenu() {
    menu = new QMenu(this);

    copyAct = new QAction(tr("&Copy\t"), this);
    menu->addAction(copyAct);
    copyAct->setEnabled(false);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySelected()));

    selectAct = new QAction(tr("&Select all\t"), this);
    menu->addAction(selectAct);
    connect(selectAct, SIGNAL(triggered()), this, SLOT(selectAll()));

    menu->addSeparator();

    clearAct = new QAction(tr("&Clear\t"), this);
    menu->addAction(clearAct);
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));
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
    delete clearAct;
    delete menu;
    delete settings;
}
