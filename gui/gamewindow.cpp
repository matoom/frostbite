#include "gamewindow.h"

#include <QDesktopServices>
#include <QToolTip>

#include "mainwindow.h"
#include "windowfacade.h"
#include "generalsettings.h"
#include "dict/dictionarysettings.h"
#include "dict/dictionaryservice.h"
#include "hyperlinkutils.h"
#include "defaultvalues.h"
#include "globaldefines.h"
#include "snapshot.h"
#include "custom/contextmenu.h"

GameWindow::GameWindow(QWidget *parent) : QPlainTextEdit(parent) {
    mainWindow = (MainWindow*)parent;       
    windowFacade = mainWindow->getWindowFacade();
    settings = GeneralSettings::getInstance();    
    dictionarySettings = DictionarySettings::getInstance();
    snapshot = new Snapshot(this);

    this->setObjectName(WINDOW_TITLE_MAIN);

    this->loadSettings();   
    this->buildContextMenu();

    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);
    this->setUndoRedoEnabled(false);
    this->setMouseTracking(true);
    this->document()->setMaximumBlockCount(GAME_WINDOW_LIMIT);

    _append = true;
    _stream = false;

    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(enableCopy(bool)));

    /* workaround for bottom margin */
    setViewportMargins(0, 0, 0, -6);
}

void GameWindow::setAppend(bool append) {
    this->_append = append;
}

bool GameWindow::append() {
    return this->_append;
}

void GameWindow::setStream(bool stream) {
    this->_stream = stream;
}

bool GameWindow::stream() {
    return this->_stream;
}

void GameWindow::showEvent(QShowEvent* event) {
    QPlainTextEdit::showEvent(event);
}

bool GameWindow::event(QEvent* event) {
    if (event->type() == QEvent::ToolTip
        && dictionarySettings->getDictOutputType() == DictionarySettings::OutputType::Tooltip) {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QTextCursor cursor = cursorForPosition(helpEvent->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        if (!cursor.selectedText().isEmpty() && !currentDictEvent.active) {
            currentDictEvent.active = true;
            currentDictEvent.word = cursor.selectedText();
            currentDictEvent.point = helpEvent->globalPos();

            mainWindow->getDictionaryService()->translate(currentDictEvent.word);
        } else {
            QToolTip::hideText();
            currentDictEvent.active = false;
        }
        return true;
    }
    return QPlainTextEdit::event(event);
}

void GameWindow::loadSettings() {
    QFont font = settings->gameWindowFont();
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);

    QPalette p = this->palette();
    p.setColor(QPalette::Text, settings->gameWindowFontColor());
    p.setColor(QPalette::Base, settings->gameWindowBackground());
    this->setPalette(p);
}

QColor GameWindow::getBgColor() {
    return viewport()->palette().color(QPalette::Base);
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
    menu = new ContextMenu(this);

    distractionFreeModeAct = new QAction(tr("Distraction free mode\t"), this);
    distractionFreeModeAct->setCheckable(true);
    distractionFreeModeAct->setChecked(false);
    menu->addAction(distractionFreeModeAct);

    menu->addSeparator();

    appearanceAct = new QAction(tr("&Appearance\t"), this);
    menu->addAction(appearanceAct);
    connect(appearanceAct, SIGNAL(triggered()), this, SLOT(changeAppearance()));

    menu->addSeparator();

    lookupDictAct = new QAction(tr("&Lookup in Dictionary\t"), this);
    menu->addAction(lookupDictAct);
    lookupDictAct->setEnabled(false);
    connect(lookupDictAct, SIGNAL(triggered()), this, SLOT(lookupInDictionary()));

    lookupWikiAct = new QAction(tr("&Lookup in Elanthipedia\t"), this);
    menu->addAction(lookupWikiAct);
    lookupWikiAct->setEnabled(false);
    connect(lookupWikiAct, SIGNAL(triggered()), this, SLOT(lookupInElanthipedia()));

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

    unstuckAct = new QAction(tr("&Unstuck\t"), this);
    menu->addAction(unstuckAct);

    menu->addSeparator();

    clearAct = new QAction(tr("&Clear\t"), this);
    menu->addAction(clearAct);
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));

    connect(distractionFreeModeAct, SIGNAL(changed()), mainWindow, SLOT(toggleDistractionFreeMode()));
}

QAction* GameWindow::getUnstuck() {
    return unstuckAct;
}

void GameWindow::changeAppearance() {
    mainWindow->openAppearanceDialog();
}

void GameWindow::saveAsHtml() {
    snapshot->save();
}

void GameWindow::contextMenuEvent(QContextMenuEvent *event) {
    QPoint point = event->globalPos();
    point.rx()--; point.ry()--;
    menu->exec(point);
}

void GameWindow::resizeEvent(QResizeEvent *event) {
    windowFacade->gameWindowResizeEvent(this);
    QPlainTextEdit::resizeEvent(event);
}

void GameWindow::mouseDoubleClickEvent(QMouseEvent* e) {
    QPlainTextEdit::mouseDoubleClickEvent(e);
    if (dictionarySettings->getDictOutputType() == DictionarySettings::OutputType::Window
        && e->button() == Qt::LeftButton
        && e->modifiers() == dictionarySettings->getDoubleClickModifier()) {
        lookupInDictionary();
    }
}

void GameWindow::mousePressEvent(QMouseEvent *e) {
    auto anchor = anchorAt(e->pos());
    if (e->button() == Qt::LeftButton && !anchor.isEmpty()) {
        clickedAnchor = anchor;
        // Here we do not call QPlainTextEdit::mousePressEvent(e);
        // due to the bug in QPlainTextEdit which picks up
        // the character format if the url was clicked.
        return;
    }
    QPlainTextEdit::mousePressEvent(e);
}

void GameWindow::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && !clickedAnchor.isEmpty()) {
        if (anchorAt(e->pos()) == clickedAnchor) {
            this->resetWindow();
            QDesktopServices::openUrl(QUrl(clickedAnchor, QUrl::TolerantMode));
        }
        clickedAnchor.clear();
        return;
    }
    QPlainTextEdit::mouseReleaseEvent(e);
}

void GameWindow::resetWindow() {
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
}

void GameWindow::mouseMoveEvent(QMouseEvent *e) {
    if(!anchorAt(e->pos()).isEmpty()) {
        viewport()->setCursor(Qt::PointingHandCursor);
    } else {
        viewport()->unsetCursor();
    }
    QPlainTextEdit::mouseMoveEvent(e);
}

void GameWindow::enableCopy(bool enabled) {
    copyAct->setEnabled(enabled);
    lookupDictAct->setEnabled(enabled);
    lookupWikiAct->setEnabled(enabled);
}


void GameWindow::lookupInDictionary() {
    QTextCursor textCursor = this->textCursor();
    if (textCursor.hasSelection()) {
        QString word = textCursor.selectedText().trimmed().toLower();
        if (word.size()) {
            mainWindow->getDictionaryService()->translate(word);
        }
    }
}

void GameWindow::lookupInElanthipedia() {
    QTextCursor textCursor = this->textCursor();
    if (textCursor.hasSelection()) {
        QString text = textCursor.selectedText().trimmed();
        if (text.size()) {
            QDesktopServices::openUrl(HyperlinkUtils::createSearchElanthipediaUrl(text));
        }
    }
}

void GameWindow::copySelected() {
    this->copy();

    QTextCursor textCursor = this->textCursor();
    textCursor.clearSelection();
    this->setTextCursor(textCursor);
}

void GameWindow::translationFinished(QString word, QString translation) {
    if (currentDictEvent.active && word == currentDictEvent.word) {
        // cut the header
        auto idx = translation.indexOf("  " + word);
        if (idx != -1) {
            QToolTip::showText(currentDictEvent.point, translation.mid(idx));
        }
        currentDictEvent.active = false;
    }
}

void GameWindow::translationFailed(QString reason) {
    (void)reason;
    QToolTip::hideText();
    currentDictEvent.active = false;    
}

GameWindow::~GameWindow() {
    delete distractionFreeModeAct;
    delete appearanceAct;
    delete lookupDictAct;
    delete lookupWikiAct;
    delete copyAct;
    delete selectAct;
    delete clearAct;
    delete saveAct;
    delete unstuckAct;
    delete menu;
    delete snapshot;
}
