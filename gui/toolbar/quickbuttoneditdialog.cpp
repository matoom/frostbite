#include "quickbuttoneditdialog.h"
#include "ui_quickbuttoneditdialog.h"

QuickButtonEditDialog::QuickButtonEditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::QuickButtonEditDialog) {
    ui->setupUi(this);

    settings = GeneralSettings::getInstance();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void QuickButtonEditDialog::updateLocation(const QPoint& point) {
    this->move(point.x(), point.y() + 50);
}

void QuickButtonEditDialog::updateButton(QToolButton* button) {
    this->button = button;
}

void QuickButtonEditDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    ui->lineEdit->setFocus();
    ui->lineEdit->setText(settings->getParameter("QuickButton/" + button->objectName(),  "").toString());
}

void QuickButtonEditDialog::okPressed() {
    settings->setParameter("QuickButton/" + button->objectName(), this->ui->lineEdit->text());
    button->setText(this->ui->lineEdit->text());
    button->setToolTip(this->ui->lineEdit->text());

    this->accept();
}

void QuickButtonEditDialog::cancelPressed() {
    this->reject();
}

QuickButtonEditDialog::~QuickButtonEditDialog() {
    delete ui;
}
