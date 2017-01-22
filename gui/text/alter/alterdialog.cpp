#include "alterdialog.h"
#include "ui_alterdialog.h"

AlterDialog::AlterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AlterDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);

    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));

    substituteTab = new SubstituteTab(this);    
    ignoreTab = new IgnoreTab(this);
}

void AlterDialog::updateSettings() {
    substituteTab->updateSettings();
    ignoreTab->updateSettings();
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

void AlterDialog::applyPressed() {
    substituteTab->saveChanges();
    ignoreTab->saveChanges();
    ui->applyButton->setEnabled(false);
}

void AlterDialog::okPressed() {
    substituteTab->saveChanges();
    ignoreTab->saveChanges();
    ui->applyButton->setEnabled(false);
    this->accept();
}

void AlterDialog::cancelPressed() {
    substituteTab->cancelChanges();
    ignoreTab->cancelChanges();
    ui->applyButton->setEnabled(false);
    this->reject();
}

AlterDialog::~AlterDialog() {
    delete substituteTab;
    delete ignoreTab;
    delete ui;
}
