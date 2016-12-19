#include "genericwindow.h"

GenericWindow::GenericWindow(QWidget *parent) : QPlainTextEdit(parent) {
    mainWindow = (MainWindow*)parent;
    settings = new GeneralSettings();
    wm = mainWindow->getWindowFacade();
    snapshot = new Snapshot(this);

    this->buildContextMenu();
    this->loadSettings();

    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);
    this->setUndoRedoEnabled(false);
    this->document()->setMaximumBlockCount(1000);

    _append = true;
    _stream = false;

    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(enableCopy(bool)));
}

void GenericWindow::setAppend(bool append) {
    this->_append = append;
}

bool GenericWindow::append() {
    return this->_append;
}

void GenericWindow::setStream(bool stream) {
    this->_stream = stream;
}

bool GenericWindow::stream() {
    return this->_stream;
}

void GenericWindow::loadSettings() {
    QFont font = settings->dockWindowFont();
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);
}

QColor GenericWindow::getBgColor() {
    return viewport()->palette().color(QPalette::Base);
}

QColor GenericWindow::getTextColor() {
    return viewport()->palette().color(QPalette::Text);
}

QTextDocument* GenericWindow::getDocument() {
    return this->document();
}

QString GenericWindow::getObjectName() {
    return this->objectName();
}

QPlainTextEdit* GenericWindow::getMainWindow() {
    return wm->getGameWindow();
}

void GenericWindow::buildContextMenu() {
    menu = new QMenu(this);

    appearanceAct = new QAction(tr("&Appearance\t"), this);
    menu->addAction(appearanceAct);
    connect(appearanceAct, SIGNAL(triggered()), this, SLOT(changeAppearance()));

    menu->addSeparator();

    copyAct = new QAction(tr("&Copy\t"), this);
    menu->addAction(copyAct);
    copyAct->setEnabled(false);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySelected()));

    selectAct = new QAction(tr("&Select all\t"), this);
    menu->addAction(selectAct);
    connect(selectAct, SIGNAL(triggered()), this, SLOT(selectAll()));

    menu->addSeparator();

    saveAct = new QAction(tr("&Save as HTML\t"), this);
    menu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveAsHtml()));

    menu->addSeparator();

    clearAct = new QAction(tr("&Clear\t"), this);
    menu->addAction(clearAct);
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));
}

void GenericWindow::changeAppearance() {
    mainWindow->openAppearanceDialog();
}

void GenericWindow::saveAsHtml() {
    snapshot->save();
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

void GenericWindow::appendHtmlStream(const QString &html) {
    QScrollBar* p_scroll_bar = this->verticalScrollBar();
    bool bool_at_bottom = (p_scroll_bar->value() == p_scroll_bar->maximum());

    QTextCursor text_cursor = QTextCursor(this->document());
    text_cursor.beginEditBlock();

    text_cursor.movePosition(QTextCursor::End);

    QStringList string_list = html.split("\n");
    for (int i = 0; i < string_list.size(); i++){
        text_cursor.insertHtml("<span class=\"body\" style=\"white-space: pre-wrap;\">" +
                               string_list.at(i) +
                               "</span>");
        if ((i + 1) < string_list.size()){
            text_cursor.insertBlock();
        }
    }

    text_cursor.endEditBlock();
    if (bool_at_bottom) {
        p_scroll_bar->setValue(p_scroll_bar->maximum());
    }
}

GenericWindow::~GenericWindow() {
    delete copyAct;
    delete selectAct;
    delete clearAct;
    delete menu;
    delete settings;
}
