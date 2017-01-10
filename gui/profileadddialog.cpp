#include "profileadddialog.h"
#include "ui_profileadddialog.h"

ProfileAddDialog::ProfileAddDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileAddDialog) {
    ui->setupUi(this);

    clientSettings = new ClientSettings();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

void ProfileAddDialog::createProfile() {
    QString folder = QApplication::applicationDirPath() + "/profiles/" + ui->lineEdit->text();
    if(!ui->lineEdit->text().isEmpty()) {
        if(!QDir(folder).exists())
            QDir().mkdir(folder);
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
    delete clientSettings;
    delete ui;
}
