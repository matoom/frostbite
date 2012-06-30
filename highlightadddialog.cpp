#include "highlightadddialog.h"
#include "ui_highlightadddialog.h"

HighlightAddDialog::HighlightAddDialog(HighlightTextTab *textTab, QWidget *parent) :
    QDialog(parent), ui(new Ui::HighlightAddDialog) {

    settings = HighlightSettings::Instance();

    this->textTab = textTab;

    ui->setupUi(this);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
}

QComboBox* HighlightAddDialog::getGroupSelect() {
    return ui->groupSelect;
}

void HighlightAddDialog::okPressed() {
    //saveChanges();

    qDebug() << "ADD::";
    HighlightSettingsEntry he = HighlightSettingsEntry("", "kurk", "combat",QColor(0, 0, 0), "play.wav", 20, "overwrite", 1);

    settings->addParameter(he);
    this->accept();
}

void HighlightAddDialog::cancelPressed() {
    this->reject();
}

HighlightAddDialog::~HighlightAddDialog() {
    delete ui;
}
