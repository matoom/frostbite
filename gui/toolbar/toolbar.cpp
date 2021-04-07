#include "toolbar.h"

#include "mainwindow.h"
#include "toolbar/toolbardefines.h"
#include "toolbar/vitalsindicator.h"
#include "toolbar/statusindicator.h"
#include "toolbar/wieldindicator.h"
#include "toolbar/spellindicator.h"
#include "toolbar/quickbuttondisplay.h"
#include "toolbar/quickbuttoneditdialog.h"
#include "toolbar/fullscreenbutton.h"
#include "toolbar/activespellindicator.h"
#include "toolbar/mutebutton.h"
#include "gamedatacontainer.h"
#include "clientsettings.h"
#include "commandline.h"
#include "textutils.h"

Toolbar::Toolbar(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    gameDataContainer = GameDataContainer::Instance();
    clientSettings = ClientSettings::getInstance();

    vitalsIndicator = new VitalsIndicator(this);
    statusIndicator = new StatusIndicator(this);
    quickButtonDisplay = new QuickButtonDisplay(this);
    wieldLeft = new WieldIndicator(this, LHAND_ICO);
    wieldRight = new WieldIndicator(this, RHAND_ICO);
    spell = new SpellIndicator(this);            
    activeSpell = new ActiveSpellIndicator(this);

    connect(mainWindow, SIGNAL(profileChanged()), this, SLOT(reloadSettings()));   
}

void Toolbar::reloadSettings() {
    quickButtonDisplay->reloadSettings();
}

MainWindow* Toolbar::getMainWindow() {
    return mainWindow;
}

void Toolbar::addMuteButton() {
    muteButton = new MuteButton(mainWindow);
    QWidget* buttonWidget = new QWidget(mainWindow);
    QHBoxLayout* hLayout = new QHBoxLayout(buttonWidget);
    buttonWidget->setLayout(hLayout);
    hLayout->addWidget((QToolButton*)muteButton);
    hLayout->setContentsMargins(25, 0, 25, 0);

    muteButtonAction = mainWindow->addToolbarWidget(buttonWidget);    

    bool mutedVisible = clientSettings->getParameter("Toolbar/muted", false).toBool();
    if(!mutedVisible) muteButtonAction->setVisible(mutedVisible);
}

void Toolbar::addFullScreenButton() {
    /* spacer forcing to float right */
    QWidget* spacerWidget = new QWidget(mainWindow);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    mainWindow->addToolbarWidget(spacerWidget);

    /* fullscreen button */
    FullscreenButton* fullScreenButton = new FullscreenButton(mainWindow);
    QWidget* buttonWidget = new QWidget(mainWindow);
    QHBoxLayout* hLayout = new QHBoxLayout(buttonWidget);
    buttonWidget->setLayout(hLayout);
    hLayout->addWidget((QToolButton*)fullScreenButton);
    mainWindow->addToolbarWidget(buttonWidget);
}

void Toolbar::loadToolbar() {
    mainWindow->setToolbarAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    QWidget* wieldLeftWidget = wieldLeft->create();
    wieldLeftWidget->setContentsMargins(QMargins(20, 0, 0, 0));
    bool wieldLeftVisible = clientSettings->getParameter("Toolbar/wieldLeft", true).toBool();
    if(!wieldLeftVisible) wieldLeftWidget->setVisible(wieldLeftVisible);
    wieldLeftAction = mainWindow->addToolbarWidget(wieldLeftWidget);

    QWidget* wieldRightWidget = wieldRight->create();
    bool wieldRightVisible = clientSettings->getParameter("Toolbar/wieldRight", true).toBool();
    if(!wieldRightVisible) wieldRightWidget->setVisible(wieldRightVisible);
    wieldRightAction = mainWindow->addToolbarWidget(wieldRightWidget);

    QWidget* spellWidget = spell->create();
    bool spellVisible = clientSettings->getParameter("Toolbar/spell", true).toBool();
    if(!spellVisible) spellWidget->setVisible(spellVisible);
    spellAction = mainWindow->addToolbarWidget(spellWidget);

    QWidget* activeSpellsWidget = activeSpell->create();
    bool activeSpellsVisible = clientSettings->getParameter("Toolbar/activeSpells", true).toBool();
    if(!activeSpellsVisible) activeSpellsWidget->setVisible(activeSpellsVisible);
    activeSpellAction = mainWindow->addToolbarWidget(activeSpellsWidget);

    QWidget* statusWidget = statusIndicator->create();
    bool statusVisible = clientSettings->getParameter("Toolbar/status", true).toBool();
    if(!statusVisible) statusWidget->setVisible(statusVisible);
    statusAction = mainWindow->addToolbarWidget(statusWidget);

    this->addMuteButton();

    QWidget* buttonsWidget = quickButtonDisplay->create();
    bool buttonsVisible = clientSettings->getParameter("Toolbar/buttons", true).toBool();
    if(!buttonsVisible) buttonsWidget->setVisible(buttonsVisible);
    buttonsAction = mainWindow->addToolbarWidget(buttonsWidget);

    QWidget* vitalsWidget = vitalsIndicator->create();
    bool vitalsVisible = clientSettings->getParameter("Toolbar/vitals", true).toBool();
    if(!vitalsVisible) vitalsWidget->setVisible(vitalsVisible);
    vitalsAction = mainWindow->addToolbarWidget(vitalsWidget);

    this->addFullScreenButton();
}

void Toolbar::updateWieldLeft(QString value) {
    wieldLeft->textLabel->setToolTip(value);
    wieldLeft->textLabel->setText(value);
}

void Toolbar::updateWieldRight(QString value) {
    wieldRight->textLabel->setToolTip(value);
    wieldRight->textLabel->setText(value);
}

void Toolbar::updateSpell(QString text) {
    spell->setText(text);
}

QHash<QString, bool> Toolbar::getStatus() {
    return statusIndicator->getFullStatus();
}

void Toolbar::updateVitals(QString name, QString value) {
    int intValue = value.toInt();
    if(name == "health") {
        gameDataContainer->setHealth(intValue);
        vitalsIndicator->healthBar->setValue(intValue);
        vitalsIndicator->healthBar->setToolTip("Health: " + value + "%");
        vitalsIndicator->healthBar->repaint();
    } else if(name == "concentration") {
        gameDataContainer->setConcentration(intValue);
        vitalsIndicator->concentrationBar->setValue(intValue);
        vitalsIndicator->concentrationBar->setToolTip("Concentration: " + value + "%");
        vitalsIndicator->concentrationBar->repaint();
    } else if(name == "stamina") {
        gameDataContainer->setFatigue(intValue);
        vitalsIndicator->fatigueBar->setValue(intValue);
        vitalsIndicator->fatigueBar->setToolTip("Fatigue: " + value + "%");
        vitalsIndicator->fatigueBar->repaint();
    } else if(name == "spirit") {
        gameDataContainer->setSpirit(intValue);
        vitalsIndicator->spiritBar->setValue(intValue);
        vitalsIndicator->spiritBar->setToolTip("Spirit: " + value + "%");
        vitalsIndicator->spiritBar->repaint();
    } else if(name == "mana") {
        gameDataContainer->setMana(intValue);
        vitalsIndicator->manaBar->setValue(intValue);
        vitalsIndicator->manaBar->setToolTip("Mana: " + value + "%");
        vitalsIndicator->manaBar->repaint();
    }
}

void Toolbar::updateStatus(QString visible, QString icon) {
    statusIndicator->updateStatus(visible, icon);
}

int Toolbar::getHealthValue() {
    return vitalsIndicator->healthBar->value();
}

int Toolbar::getConcentrationValue() {
    return vitalsIndicator->concentrationBar->value();
}

int Toolbar::getFatigueValue() {
    return vitalsIndicator->fatigueBar->value();
}

int Toolbar::getSpiritValue() {
    return vitalsIndicator->spiritBar->value();
}

void Toolbar::quickButtonAction() {
    QToolButton *button = (QToolButton *)sender();

    mainWindow->getCommandLine()->setText(button->text());
    emit mainWindow->getCommandLine()->sendCommand();
}

void Toolbar::setWieldLeftVisible(bool visible) {
    wieldLeftAction->setVisible(visible);
}

void Toolbar::setWieldRightVisible(bool visible) {
    wieldRightAction->setVisible(visible);
}

void Toolbar::setSpellVisible(bool visible) {
    spellAction->setVisible(visible);
}

void Toolbar::setActiveSpellsVisible(bool visible) {
    activeSpellAction->setVisible(visible);
}

void Toolbar::setStatusVisible(bool visible) {
    statusAction->setVisible(visible);
}

void Toolbar::setButtonsVisible(bool visible) {
    buttonsAction->setVisible(visible);
}

void Toolbar::setVitalsVisible(bool visible) {
    vitalsAction->setVisible(visible);
}

void Toolbar::setMuteVisible(bool visible) {
    muteButtonAction->setVisible(visible);
}

void Toolbar::updateActiveSpells(QStringList activeSpells) {
    activeSpell->setText(TextUtils::findLowestActiveValue(activeSpells));

    QString text = "<table>";
    foreach(QString activeSpell, activeSpells) {
        text += "<tr><td><div style='white-space: pre;'>" + activeSpell + "</div><td/></tr>";
    }
    text += "</table>";

    activeSpell->setToolTip(text);
}

void Toolbar::clearActiveSpells() {
    activeSpell->setText("-");
    activeSpell->setToolTip("None");
}

Toolbar::~Toolbar() {
    delete vitalsIndicator;
    delete statusIndicator;
    delete quickButtonDisplay;
    delete wieldLeft;
    delete wieldRight;
    delete activeSpell;
    delete muteButton;
}
