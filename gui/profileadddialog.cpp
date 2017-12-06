#include "profileadddialog.h"
#include "ui_profileadddialog.h"

ProfileAddDialog::ProfileAddDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileAddDialog) {
    ui->setupUi(this);

    clientSettings = ClientSettings::getInstance();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    this->populate();
}

void ProfileAddDialog::populate() {
    ui->locationSelect->addItem("User Home", 0);    
    ui->locationSelect->addItem("Local", 1);
    ui->locationSelect->setToolTip("User Home: " + QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/profiles/\n" +
                                   "Local: " + QApplication::applicationDirPath() + "/profiles/");
}

void ProfileAddDialog::createProfile() {
    QString folder;
    if(ui->locationSelect->currentData() == 0) {
        folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    } else {
        folder = QApplication::applicationDirPath();
    }
    folder += "/profiles/" + ui->lineEdit->text();

    if(!ui->lineEdit->text().isEmpty()) {
        QDir().mkpath(folder);
    }
    emit updateMenu();
}

void ProfileAddDialog::okClicked() {
    this->createProfile();
    this->accept();
}

void ProfileAddDialog::cancelClicked() {
    this->reject();
}

ProfileAddDialog::~ProfileAddDialog() {
    delete ui;
}
