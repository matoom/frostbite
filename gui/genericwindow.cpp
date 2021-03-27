#include "genericwindow.h"

GenericWindow::GenericWindow(QString title, QWidget *parent) : QPlainTextEdit(parent) {
    mainWindow = (MainWindow*)parent;
    settings = GeneralSettings::getInstance();
    dictionarySettings = DictionarySettings::getInstance();
    wm = mainWindow->getWindowFacade();
    snapshot = new Snapshot(this);

    this->windowId = title.simplified().remove(' ') + "Window";

    this->buildContextMenu();
    this->loadSettings();

    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);
    this->setUndoRedoEnabled(false);
    this->document()->setMaximumBlockCount(1000);

    _append = true;
    _stream = false;

    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(enableCopy(bool)));
    connect(mainWindow->getWindowFacade(), SIGNAL(updateWindowSettings()), this, SLOT(updateSettings()));
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

void GenericWindow::updateSettings() {
    settings = GeneralSettings::getInstance();
    this->loadSettings();
}

void GenericWindow::loadSettings() {
    QVariant fontValue = settings->getParameter(windowId + "/font", QVariant());
    QFont font;
    if(!fontValue.isNull()) {
       font = fontValue.value<QFont>();
       this->setProperty(WINDOW_FONT_ID, font);
       fontAct->setText(font.family() + " " + QString::number(font.pointSize()));
    } else {
       font = settings->dockWindowFont();
       this->setProperty(WINDOW_FONT_ID, QVariant());
       fontAct->setText(WINDOW_FONT_SET);
    }
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
    menu = new ContextMenu(this);
    appearanceAct = new QAction(tr("&Appearance\t"), this);
    menu->addAction(appearanceAct);
    connect(appearanceAct, SIGNAL(triggered()), this, SLOT(changeAppearance()));

    lookupDictAct = new QAction(tr("&Lookup in Dictionary\t"), this);
    menu->addAction(lookupDictAct);
    lookupDictAct->setEnabled(false);
    connect(lookupDictAct, SIGNAL(triggered()), this, SLOT(lookupInDictionary()));

    menu->addSeparator();

    fontAct = new QAction(tr("&" WINDOW_FONT_SET "\t"), this);
    QVariant fontValue = settings->getParameter(windowId + "/font", QVariant());
    if(!fontValue.isNull()) {
        QFont font = fontValue.value<QFont>();
        fontAct->setText(font.family() + " " + QString::number(font.pointSize()));
    }

    menu->addAction(fontAct);
    connect(fontAct, SIGNAL(triggered()), this, SLOT(selectFont()));

    clearFontAct = new QAction(tr("&" WINDOW_FONT_CLEAR "\t"), this);
    menu->addAction(clearFontAct);
    connect(clearFontAct, SIGNAL(triggered()), this, SLOT(clearFont()));

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

void GenericWindow::selectFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this->font(), this);
    if(ok) {
        fontAct->setText(font.family() + " " + QString::number(font.pointSize()));
        settings->setParameter(windowId + "/font", font);
        this->setProperty(WINDOW_FONT_ID, font);
        this->setFont(font);
    }
}

void GenericWindow::clearFont() {
    QFont font = settings->getParameter("DockWindow/font",
        QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE)).value<QFont>();
    this->setFont(font);
    fontAct->setText(WINDOW_FONT_SET);
    settings->setParameter(windowId + "/font", QVariant());
    this->setProperty(WINDOW_FONT_ID, QVariant());
}

void GenericWindow::changeAppearance() {
    mainWindow->openAppearanceDialog();
}

void GenericWindow::saveAsHtml() {
    snapshot->save();
}

void GenericWindow::contextMenuEvent(QContextMenuEvent* event) {
    QPoint point = event->globalPos();
    point.rx()--; point.ry()--;
    menu->exec(point);
}

void GenericWindow::mouseDoubleClickEvent(QMouseEvent *e) {
    QPlainTextEdit::mouseDoubleClickEvent(e);
    if (dictionarySettings->getDoubleClickEnabled() &&
        e->button() == Qt::LeftButton &&
        e->modifiers() == dictionarySettings->getDoubleClickModifier()) {
        lookupInDictionary();
    }
}

void GenericWindow::enableCopy(bool enabled) {
    copyAct->setEnabled(enabled);
    lookupDictAct->setEnabled(enabled);    
}

void GenericWindow::lookupInDictionary() {
  QTextCursor textCursor = this->textCursor();
  if (textCursor.hasSelection()) {
    QString word = textCursor.selectedText().trimmed().toLower();
    if (word.size()) {
        mainWindow->getDictionaryService()->translate(word);
    }
  }
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
    for (int i = 0; i < string_list.size(); i++) {
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
    delete fontAct;
    delete clearFontAct;
    delete menu;
    delete snapshot;    
}
