#include "highlightadddialog.h"
#include "ui_highlightadddialog.h"

HighlightAddDialog::HighlightAddDialog(HighlightTextTab *textTab, QWidget *parent) :
    QDialog(parent), ui(new Ui::HighlightAddDialog) {

    parentDialog = (QDialog*)parent;

    settings = new HighlightSettings();

    this->textTab = textTab;

    ui->setupUi(this);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
}

void HighlightAddDialog::updateSettings() {
    settings->init();
}

void HighlightAddDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);
    this->move(parentDialog->x() + (parentDialog->height() / 4),
               parentDialog->y() + (parentDialog->width() / 4));
    ui->textLine->setFocus();
}

QComboBox* HighlightAddDialog::getGroupSelect() {
    return ui->groupSelect;
}

void HighlightAddDialog::okPressed() {
    QString group = ui->groupSelect->currentText();
    QString text = ui->textLine->text();

    QColor color(255, 255, 255);
    if (textTab->bgColor.value() > 150) {
        color.setRgb(0, 0, 0);
    }

    HighlightSettingsEntry entry = HighlightSettingsEntry(NULL, text, group,
        color, NULL, NULL, NULL, NULL, NULL, QBitArray(3));

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
    delete settings;
}
