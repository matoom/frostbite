#include "toolbar.h"

Toolbar::Toolbar(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    gameDataContainer = GameDataContainer::Instance();
    textUtils = TextUtils::Instance();

    vitalsIndicator = new VitalsIndicator(this);
    statusIndicator = new StatusIndicator(this);
    quickButtonDisplay = new QuickButtonDisplay(this);
    wieldLeft = new WieldIndicator(this, LHAND_ICO);
    wieldRight = new WieldIndicator(this, RHAND_ICO);
    spell = new SpellIndicator(this);            
    activeSpell = new ActiveSpellIndicator(this);
}

void Toolbar::updateQuickButtonSettings() {
    quickButtonDisplay->updateSettings();
    quickButtonDisplay->reloadSettings();
}

MainWindow* Toolbar::getMainWindow() {
    return mainWindow;
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
    mainWindow->addToolbarWidget(wieldLeftWidget);
    mainWindow->addToolbarWidget(wieldRight->create());
    mainWindow->addToolbarWidget(spell->create());
    mainWindow->addToolbarWidget(activeSpell->create());

    mainWindow->addToolbarWidget(statusIndicator->create());

    mainWindow->addToolbarWidget(quickButtonDisplay->create());

    mainWindow->addToolbarWidget(vitalsIndicator->create());

    this->addFullScreenButton();
}

void Toolbar::updateWieldLeft(QString value) {
    wieldLeft->textLabel->setText(value);
}

void Toolbar::updateWieldRight(QString value) {
    wieldRight->textLabel->setText(value);
}

void Toolbar::updateSpell(QString toolTip) {
    spell->setToolTip("<table style='margin: 2px;'><tr><td>" + toolTip + "</td></tr></table>");
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

void Toolbar::updateActiveSpells(QStringList activeSpells) {
    activeSpell->setText(textUtils->findLowestActiveValue(activeSpells), QString::number(activeSpells.count()));

    QString text = "<table style='margin: 4px;'>";
    foreach(QString activeSpell, activeSpells) {
        text += "<tr><td><div style='font-size: 14px; white-space: pre;'>" + activeSpell + "</div><td/></tr>";
    }
    text += "</table>";

    activeSpell->setToolTip(text);
}

void Toolbar::clearActiveSpells() {
    activeSpell->setText("-", "-");
    activeSpell->setToolTip("None");
}

Toolbar::~Toolbar() {
    delete vitalsIndicator;
    delete statusIndicator;
    delete quickButtonDisplay;
    delete wieldLeft;
    delete wieldRight;
    delete activeSpell;
}
