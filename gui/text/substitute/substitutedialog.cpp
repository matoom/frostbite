#include "substitutedialog.h"
#include "ui_substitutedialog.h"

SubstituteDialog::SubstituteDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SubstituteDialog) {
    ui->setupUi(this);
}

SubstituteDialog::~SubstituteDialog() {
    delete ui;
}
