#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    this->addVersion();
}

void AboutDialog::addVersion() {
    QString releaseVersion = QCoreApplication::applicationVersion();
    QString text = ui->textEdit->toPlainText();
    text.replace("${RELEASE_VERSION}", releaseVersion);
    ui->textEdit->setText(text);
}

void AboutDialog::close() {
    this->accept();
}

AboutDialog::~AboutDialog() {
    delete ui;
}
