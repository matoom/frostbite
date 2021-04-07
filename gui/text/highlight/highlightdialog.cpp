#include "highlightdialog.h"
#include "ui_highlightdialog.h"

#include "mainwindow.h"
#include "clientsettings.h"

#include "text/highlight/highlightgeneraltab.h"
#include "text/highlight/highlighttexttab.h"
#include "text/highlight/highlightalerttab.h"

#include "windowfacade.h"

HighlightDialog::HighlightDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HighlightDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);
    settings = ClientSettings::getInstance();

    generalTab = new HighlightGeneralTab(this);
    textTab = new HighlightTextTab(this);
    alertTab = new HighlightAlertTab(this);

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
}

MainWindow* HighlightDialog::getMainWindow() {
    return mainWindow;
}

void HighlightDialog::reloadTextHighlights() {
    textTab->updateSettings();
    textTab->reloadHighlightList();
}

void HighlightDialog::updateSettings() {
    generalTab->updateSettings();
    textTab->updateSettings();
    alertTab->updateSettings();
}

void HighlightDialog::loadSettings() {
    generalTab->loadSettings();
    generalTab->prepareList();

    textTab->reloadHighlightList();
    alertTab->initSettings();
}

QListWidget* HighlightDialog::getGeneralList() {
    return ui->gList;
}

QGroupBox* HighlightDialog::getGeneralAlertGroup() {
    return ui->gAlertGroup;
}

QComboBox* HighlightDialog::getGeneralFileSelect() {
    return ui->gFileSelect;
}

QPushButton* HighlightDialog::getGeneralPlayButton() {
        return ui->gPlay;
}

QPushButton* HighlightDialog::getApplyButton() {
    return ui->applyButton;
}

QPushButton* HighlightDialog::getTextAddButton() {
    return ui->tAdd;
}

QPushButton* HighlightDialog::getTextRemoveButton() {
    return ui->tRemove;
}

QListWidget* HighlightDialog::getTextList() {
    return ui->tList;
}

QComboBox* HighlightDialog::getTextHighlightGroup() {
    return ui->tGroup;
}

QComboBox* HighlightDialog::getTextHighlightSortBy() {
    return ui->tSortBy;
}

QLineEdit* HighlightDialog::getTextHighlightFilter() {
    return ui->tFilterEdit;
}

QComboBox* HighlightDialog::getTextFileSelect() {
    return ui->tFileSelect;
}

QPushButton* HighlightDialog::getTextPlayButton() {
    return ui->tPlay;
}

QComboBox* HighlightDialog::getTextActionSelect(){
    return ui->tActionSelect;
}

QGroupBox* HighlightDialog::getTextTimerGroup(){
    return ui->tTimerGroup;
}

QLineEdit* HighlightDialog::getTextTimerValue(){
    return ui->tTimerValue;
}

QGroupBox* HighlightDialog::getTextAlertGroup(){
    return ui->tAlertGroup;
}

QCheckBox* HighlightDialog::getTextEntireRow(){
    return ui->tEntireRow;
}

QCheckBox* HighlightDialog::getTextPartialMatch(){
    return ui->tPartialMatch;
}

QCheckBox* HighlightDialog::getTextGroupsMatch(){
    return ui->tGroupsMatch;
}

QCheckBox* HighlightDialog::getTextCaseInsensitive(){
    return ui->tCaseInsensitive;
}

QCheckBox* HighlightDialog::getTextStartingWith(){
    return ui->tStartingWith;
}

QGroupBox* HighlightDialog::getBleedingGroup(){
    return ui->bleedingGroup;
}

QPushButton* HighlightDialog::getBleedingPlayButton(){
    return ui->bleedingPlayButton;
}

QComboBox* HighlightDialog::getBleedingPlaySelect(){
    return ui->bleedingPlaySelect;
}

QGroupBox* HighlightDialog::getStunGroup(){
    return ui->stunGroup;
}

QPushButton* HighlightDialog::getStunPlayButton(){
    return ui->stunPlayButton;
}

QComboBox* HighlightDialog::getStunSelect(){
    return ui->stunPlaySelect;
}

QGroupBox* HighlightDialog::getHealthGroup(){
    return ui->healthGroup;
}

QPushButton* HighlightDialog::getHealthPlayButton(){
    return ui->healthPlayButton;
}

QComboBox* HighlightDialog::getHealthSelect(){
    return ui->healthPlaySelect;
}

QSlider* HighlightDialog::getHealthSlider(){
    return ui->healthSlider;
}

QLabel* HighlightDialog::getHealthSliderText(){
    return ui->healthSliderText;
}

QGroupBox* HighlightDialog::getDeathGroup(){
    return ui->deathGroup;
}

QPushButton* HighlightDialog::getDeathPlayButton(){
    return ui->deathPlayButton;
}

QComboBox* HighlightDialog::getDeathSelect(){
    return ui->deathPlaySelect;
}

QGroupBox* HighlightDialog::getWebbedGroup(){
    return ui->webbedGroup;
}

QPushButton* HighlightDialog::getWebbedPlayButton() {
    return ui->webbedPlayButton;
}

QComboBox* HighlightDialog::getWebbedSelect(){
    return ui->webbedPlaySelect;
}

void HighlightDialog::reloadWindowStyles() {
    mainWindow->getWindowFacade()->updateWindowStyle();
}

void HighlightDialog::reloadHighlighterSettings() {
    mainWindow->getWindowFacade()->reloadWindowSettings();
}

void HighlightDialog::okPressed() {
    generalTab->saveChanges();
    textTab->saveChanges();
    alertTab->saveChanges();

    ui->applyButton->setEnabled(false);

    this->accept();
}

void HighlightDialog::cancelPressed() {
    generalTab->cancelChanges();
    textTab->cancelChanges();
    alertTab->cancelChanges();

    ui->applyButton->setEnabled(false);

    this->reject();
}

void HighlightDialog::applyPressed() {
    generalTab->saveChanges();
    textTab->saveChanges();
    alertTab->saveChanges();

    ui->applyButton->setEnabled(false);
}

HighlightDialog::~HighlightDialog() {
    delete ui;
}
