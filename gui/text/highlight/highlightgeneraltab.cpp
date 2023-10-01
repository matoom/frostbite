#include "highlightgeneraltab.h"

#include "text/highlight/highlightdialog.h"
#include "text/highlight/highlightsettings.h"
#include "audio/audioplayer.h"
#include "defaultvalues.h"
#include "generalsettings.h"
#include "custom/contextmenu.h"
#include "mainwindow.h"

HighlightGeneralTab::HighlightGeneralTab(QObject *parent) : QObject(parent) {
    highlightDialog = (HighlightDialog*)parent;
    settings = HighlightSettings::getInstance();
    generalSettings = GeneralSettings::getInstance();
    audioPlayer = new AudioPlayer(highlightDialog->getMainWindow());

    listWidget = highlightDialog->getGeneralList();
    alertGroup = highlightDialog->getGeneralAlertGroup();
    fileSelect = highlightDialog->getGeneralFileSelect();
    playButton = highlightDialog->getGeneralPlayButton();

    applyButton = highlightDialog->getApplyButton();

    this->loadSettings();
    this->prepareList();

    this->initFileSelect();
    this->initContextMenu();
    this->setBackground();

    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(itemSelected(QListWidgetItem*, QListWidgetItem*)));
    connect(listWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(listWidgetMenuRequested(const QPoint &)));
    connect(alertGroup, SIGNAL(clicked(bool)), this, SLOT(alertClicked(bool)));
    connect(fileSelect, SIGNAL(activated(const QString&)), this,
            SLOT(fileSelected(const QString&)));    
    connect(playButton, SIGNAL(clicked()), this, SLOT(playSound()));
}

void HighlightGeneralTab::setBackground() {
    QColor bgColor = generalSettings->gameWindowBackground();
    listWidget->setStyleSheet("QListWidget {background-color:" + bgColor.name() + ";}");
}

void HighlightGeneralTab::updateSettings() {
    settings = HighlightSettings::getInstance();
    generalSettings = GeneralSettings::getInstance();

    this->loadSettings();
    this->setBackground();
    listWidget->clear();
}

void HighlightGeneralTab::loadSettings() {
    highlightList.insert(ROOM_NAME, readSettings(ROOM_NAME, "[Room titles]", QColor(ROOM_NAME_COLOR_HEX)));
    highlightList.insert(GAME_MESSAGE, readSettings(GAME_MESSAGE, "Game Messages", QColor(GAME_MESSAGE_COLOR_HEX)));
    highlightList.insert(DAMAGE, readSettings(DAMAGE, "Taking damage in combat", QColor(DAMAGE_COLOR_HEX)));
    highlightList.insert(SPEECH, readSettings(SPEECH, "Someone says, ..", QColor(SPEECH_COLOR_HEX)));
    highlightList.insert(WHISPER, readSettings(WHISPER, "Someone whispers, ..", QColor(WHISPER_COLOR_HEX)));
    highlightList.insert(THINKING, readSettings(THINKING, "Your mind hears someone thinking, .. ", QColor(THINKING_COLOR_HEX)));
    highlightList.insert(BONUS, readSettings(BONUS, "Stat bonus", QColor(BOOST_COLOR_HEX)));
    highlightList.insert(PENALTY, readSettings(PENALTY, "Stat penalty", QColor(PENALTY_COLOR_HEX)));
    highlightList.insert(SCRIPT, readSettings(SCRIPT, "Script commands", QColor(SCRIPT_COLOR_HEX)));
    highlightList.insert(ECHO, readSettings(ECHO, "Script echo", QColor(ECHO_COLOR_HEX)));
    highlightList.insert(LINK, readSettings(LINK, "Links", QColor(LINK_COLOR_HEX)));
}

void HighlightGeneralTab::initContextMenu() {
    menu = new ContextMenu(listWidget);
    colorAct = new QAction(QIcon(":/window/images/color.png"), tr("&Change Color ..."), listWidget);
    menu->addAction(colorAct);
    connect(colorAct, SIGNAL(triggered()), this, SLOT(colorDialog()));

    menu->addSeparator();

    bgColorAct = new QAction(QIcon(":/window/images/color.png"), tr("&Change Background ..."), listWidget);
    menu->addAction(bgColorAct);
    connect(bgColorAct, SIGNAL(triggered()), this, SLOT(bgColorDialog()));

    bgClearAct = new QAction(QIcon(), tr("&Clear Background"), listWidget);
    menu->addAction(bgClearAct);
    connect(bgClearAct, SIGNAL(triggered()), this, SLOT(clearBgColor()));

    menu->addSeparator();

    editAct = new QAction(QIcon(":/window/images/edit.png"), tr("&Edit..."), listWidget);
    menu->addAction(editAct);
    editAct->setEnabled(false);
}

void HighlightGeneralTab::colorDialog() {
    QColor color = QColorDialog::getColor(listWidget->currentItem()->textColor(), listWidget, tr("Select color"));

    if (color.isValid()) {
        QString currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toString();
        listWidget->currentItem()->setTextColor(color);
        updateSelectedItemColor(listWidget->currentItem());
        highlightList[currentItemKey].insert("color", color);

        this->registerChange(currentItemKey);
    }
}

void HighlightGeneralTab::bgColorDialog() {
    QColor color = QColorDialog::getColor(listWidget->currentItem()->textColor(),
        listWidget, tr("Select background color"));

    if (color.isValid()) {
        QString currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toString();
        listWidget->currentItem()->setBackgroundColor(color);
        updateSelectedItemColor(listWidget->currentItem());
        highlightList[currentItemKey].insert("bgColor", color);
        this->registerChange(currentItemKey);
    }
}

void HighlightGeneralTab::clearBgColor() {
    QColor color = QColor();

    QString currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toString();
    listWidget->currentItem()->setBackgroundColor(QColor(Qt::transparent));
    updateSelectedItemColor(listWidget->currentItem());
    highlightList[currentItemKey].insert("bgColor", color);
    this->registerChange(currentItemKey);
}


void HighlightGeneralTab::listWidgetMenuRequested(const QPoint &point) {
    QPoint globalPos = listWidget->mapToGlobal(point);
    globalPos.rx()--; globalPos.ry()--;
    menu->exec(globalPos);
}

void HighlightGeneralTab::initFileSelect() {
    fileSelect->addItems(audioPlayer->getAudioList());
}

void HighlightGeneralTab::playSound() {
    audioPlayer->play(fileSelect->currentText());
}

void HighlightGeneralTab::prepareList() {    
    QMap<QString, QHash<QString, QVariant> >::const_iterator i = highlightList.constBegin();
    while (i != highlightList.constEnd()) {
        QListWidgetItem *newItem = new QListWidgetItem(QIcon(":/window/images/icon_ph.png"),
            tr(i.value().value("name").toByteArray().data()), listWidget);
        newItem->setData(Qt::UserRole, i.key());
        newItem->setTextColor(i.value().value("color").value<QColor>());
        QColor bgColor = i.value().value("bgColor").value<QColor>();
        if(bgColor.isValid()) newItem->setBackgroundColor(bgColor);
        newItem->setFont(QFont(DEFAULT_FONT, 12));
        ++i;
    }
}

QHash<QString, QVariant> HighlightGeneralTab::readSettings(QString id, QString name, QColor defaultColor) {
    QString nameValue = settings->getSingleParameter("GeneralHighlight/" + id + "/name", "").toString();
    QColor colorValue = settings->getSingleParameter("GeneralHighlight/" + id + "/color", DEFAULT_MAIN_FONT_COLOR).value<QColor>();
    QColor bgColorValue = settings->getSingleParameter("GeneralHighlight/" + id + "/bgColor", QColor()).value<QColor>();
    QString alertValue = settings->getSingleParameter("GeneralHighlight/" + id + "/alert", "").toString();

    QHash<QString, QVariant> item;
    item.insert("name", name);
    item.insert("color", nameValue.isEmpty() ? defaultColor : colorValue);
    item.insert("bgColor", bgColorValue);
    item.insert("alert", alertValue);

    if(nameValue.isEmpty()) {
        settings->setSingleParameter("GeneralHighlight/" + id + "/name", name);
        settings->setSingleParameter("GeneralHighlight/" + id + "/color", defaultColor);
    }
    return item;
}

void HighlightGeneralTab::registerChange(QString currentItemKey) {
    if(!generalChangeList.contains(currentItemKey)) {
        generalChangeList.append(currentItemKey);

        if(!applyButton->isEnabled()) {
            applyButton->setEnabled(true);
        }
    }
}

void HighlightGeneralTab::updateAlertSettings(QString key, QVariant value) {
    QString currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toString();
    QHash<QString, QVariant> itemSettings = highlightList.value(currentItemKey);

    QHash<QString, QVariant> alertSettings = itemSettings.value("alert").toHash();
    alertSettings.insert(key, value);

    highlightList[currentItemKey].insert("alert", alertSettings);

    registerChange(currentItemKey);
}

void HighlightGeneralTab::alertClicked(bool on) {
    updateAlertSettings("enabled", on);
}

void HighlightGeneralTab::fileSelected(const QString& text) {
    updateAlertSettings("file", text);
}

void HighlightGeneralTab::itemSelected(QListWidgetItem *current, QListWidgetItem *previous) {
    /* change highlight selected item icon */
    updateIcon(current, previous);

    /* change highlight color to item color */
    updateSelectedItemColor(current);

    /* enable/disable controls for list item */
    updateControls(current);
}

void HighlightGeneralTab::updateSelectedItemColor(QListWidgetItem *current) {
    if(current != NULL) {
        /* change highlight color to item color */
        QPalette palette = listWidget->palette();
        palette.setColor(QPalette::HighlightedText, current->textColor());
        if(current->backgroundColor().isValid()) {
            palette.setColor(QPalette::Highlight, current->backgroundColor());
            palette.setColor(QPalette::Background, current->backgroundColor());
        } else {
            palette.setColor(QPalette::Highlight, Qt::transparent);
            palette.setColor(QPalette::Base, Qt::transparent);
        }
        listWidget->setPalette(palette);
    }
}

void HighlightGeneralTab::updateIcon(QListWidgetItem *current, QListWidgetItem *previous) {
    if(current) {
        if(previous) {
            previous->setIcon(QIcon(":/window/images/icon_ph.png"));
        }
        current->setIcon(QIcon(":/window/images/arrow_right.png"));
    }
}

void HighlightGeneralTab::updateControls(QListWidgetItem *current) {
    if(current != NULL) {
        QString key = current->data(Qt::UserRole).toString();
        QHash<QString, QVariant> itemSettings = highlightList.value(key);
        QHash<QString, QVariant> alertSettings = itemSettings.value("alert").toHash();

        if(!alertSettings.isEmpty()) {
            alertGroup->setChecked(alertSettings.value("enabled").toBool());

            int index = fileSelect->findText(alertSettings.value("file").toString());
            if(index != -1) {
                fileSelect->setCurrentIndex(index);
            } else {
                fileSelect->setCurrentIndex(0);
            }
        } else {
            alertGroup->setChecked(false);
            fileSelect->setCurrentIndex(0);
        }
    } else {
        this->clearControls();
    }
}

void HighlightGeneralTab::clearControls() {
    alertGroup->setChecked(false);
    alertGroup->setDisabled(true);
}

void HighlightGeneralTab::saveChanges() {
    if(!generalChangeList.isEmpty()) {
        foreach(QString s, generalChangeList) {
            QHashIterator<QString, QVariant> i(highlightList.value(s));
            while (i.hasNext()) {
                i.next();
                settings->setSingleParameter("GeneralHighlight/" + s + "/" + i.key(), i.value());
            }
        }

        highlightDialog->reloadWindowStyles();
        generalChangeList.clear();
    }
}

void HighlightGeneralTab::cancelChanges() {
    if(!generalChangeList.isEmpty()) {
        listWidget->clear();
        this->loadSettings();
        this->prepareList();

        generalChangeList.clear();
    }
}

HighlightGeneralTab::~HighlightGeneralTab() {
    delete audioPlayer;
}
