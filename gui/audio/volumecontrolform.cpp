#include "volumecontrolform.h"
#include "ui_volumecontrolform.h"

VolumeControlForm::VolumeControlForm(QWidget *parent) : QWidget(parent), ui(new Ui::VolumeControlForm) {
    ui->setupUi(this);
    this->hide();
}

void VolumeControlForm::focusOutEvent(QFocusEvent* event) {
    qDebug() << "focusout";
    this->hide();
    QWidget::focusOutEvent(event);
}

VolumeControlForm::~VolumeControlForm() {
    delete ui;
}
