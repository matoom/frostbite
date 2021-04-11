#include "alterdialog.h"
#include "ui_alterdialog.h"

#include "windowfacade.h"

AlterDialog::AlterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AlterDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);

    ui->applyButton->setEnabled(false);

    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));

    substituteTab = new SubstituteTab(this);    
    ignoreTab = new IgnoreTab(this);
    linksTab = new LinksTab(this);
}

void AlterDialog::updateSettings() {
    substituteTab->updateSettings();
    ignoreTab->updateSettings();
    linksTab->updateSettings();
}

QList<QDockWidget*> AlterDialog::getDockWindows() {
    return mainWindow->getWindowFacade()->getDockWindows();
}

QPushButton* AlterDialog::getSubstitutionAddButton() {
    return ui->sAddButton;
}

QPushButton* AlterDialog::getSubstitutionRemoveButton() {
    return ui->sRemoveButton;
}

QPushButton* AlterDialog::getApplyButton() {
    return ui->applyButton;
}

QTableWidget* AlterDialog::getSubstitutionTable() {
    return ui->substitutionTable;
}

QPushButton* AlterDialog::getIgnoreAddButton() {
    return ui->iAddButton;
}

QPushButton* AlterDialog::getIgnoreRemoveButton() {
    return ui->iRemoveButton;
}

QTableWidget* AlterDialog::getIgnoreTable() {
    return ui->ignoreTable;
}

QCheckBox* AlterDialog::getIgnoreEnabled() {
    return ui->ignoreEnabled;
}

QTableWidget* AlterDialog::getLinksTable() {
    return ui->linksTable;
}

QPushButton* AlterDialog::getLinksAddButton() {
    return ui->lAddButton;
}

QPushButton* AlterDialog::getLinksRemoveButton() {
    return ui->lRemoveButton;
}

void AlterDialog::reloadSettings() {
    mainWindow->getWindowFacade()->reloadWindowSettings();
}

void AlterDialog::applyPressed() {
    substituteTab->saveChanges();
    ignoreTab->saveChanges();
    linksTab->saveChanges();
    this->reloadSettings();
    ui->applyButton->setEnabled(false);
}

void AlterDialog::okPressed() {
    substituteTab->saveChanges();
    ignoreTab->saveChanges();
    linksTab->saveChanges();
    this->reloadSettings();
    ui->applyButton->setEnabled(false);
    this->accept();
}

void AlterDialog::cancelPressed() {
    substituteTab->cancelChanges();
    ignoreTab->cancelChanges();
    linksTab->cancelChanges();
    ui->applyButton->setEnabled(false);
    this->reject();
}

AlterDialog::~AlterDialog() {
    delete substituteTab;
    delete ignoreTab;
    delete linksTab;
    delete ui;
}
