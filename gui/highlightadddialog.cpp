#include "highlightadddialog.h"
#include "ui_highlightadddialog.h"

HighlightAddDialog::HighlightAddDialog(HighlightTextTab *textTab, QWidget *parent) :
    QDialog(parent), ui(new Ui::HighlightAddDialog) {

    parenDialog = (QDialog*)parent;

    settings = HighlightSettings::Instance();

    this->textTab = textTab;

    ui->setupUi(this);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
}

void HighlightAddDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);
    this->move(parenDialog->x() + (parenDialog->height() / 4),
               parenDialog->y() + (parenDialog->width() / 4));
    ui->textLine->setFocus();
}

QComboBox* HighlightAddDialog::getGroupSelect() {
    return ui->groupSelect;
}

void HighlightAddDialog::okPressed() {
    QString group = ui->groupSelect->currentText();
    QString text = ui->textLine->text();

    HighlightSettingsEntry entry = HighlightSettingsEntry(NULL, text, group,
        QColor(255, 255, 255), NULL, NULL, NULL, NULL, NULL, QBitArray(3));

    settings->addParameter("TextHighlight", entry);

    textTab->reloadHighlightList();

    ui->textLine->setText("");

    this->accept();
}

void HighlightAddDialog::cancelPressed() {
    this->reject();
}

HighlightAddDialog::~HighlightAddDialog() {
    delete ui;
}
