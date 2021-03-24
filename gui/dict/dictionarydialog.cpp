#include "dictionarydialog.h"
#include "dictionarysettings.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

DictionaryDialog::DictionaryDialog(QWidget *parent) : QDialog(parent), settings(DictionarySettings::getInstance()) {
    setWindowTitle(tr("Dictionary settings"));

    QPushButton *okButton = new QPushButton(tr("Ok"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));

    dictNameEdit = new QLineEdit("", this);
    dictArgumentsEdit = new QLineEdit("", this);

    loadSettings();
    
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("Dictionary program name:")), 0, 0);
    mainLayout->addWidget(dictNameEdit, 0, 1, 1, 3);
    mainLayout->addWidget(new QLabel(tr("Command line arguments:")), 1, 0);
    mainLayout->addWidget(dictArgumentsEdit, 1, 1, 1, 3);
    mainLayout->addWidget(okButton, 2, 2);
    mainLayout->addWidget(cancelButton, 2, 3);    
}

DictionaryDialog::~DictionaryDialog() {
}

void DictionaryDialog::okPressed() {
    this->saveChanges();
    this->loadSettings();
    this->accept();
}

void DictionaryDialog::cancelPressed() {
    this->loadSettings();
    this->reject();
}

void DictionaryDialog::loadSettings() {
    dictNameEdit->setText(settings->getDictCommand());
    dictArgumentsEdit->setText(settings->getDictArguments());
}


void DictionaryDialog::saveChanges() {
  settings->setDictCommand(dictNameEdit->text())
      .setDictArguments(dictArgumentsEdit->text());
}
