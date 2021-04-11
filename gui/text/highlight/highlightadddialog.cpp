#include "highlightadddialog.h"
#include "ui_highlightadddialog.h"

#include "text/highlight/highlighttexttab.h"
#include "text/highlight/highlightsettings.h"
#include "text/highlight/highlightsettingsentry.h"
#include "text/highlight/sortablelistwidgetitem.h"

HighlightAddDialog::HighlightAddDialog(HighlightTextTab *textTab, QWidget *parent) :
    QDialog(parent), ui(new Ui::HighlightAddDialog) {

    parentDialog = (QDialog*)parent;

    settings = HighlightSettings::getInstance();

    this->textTab = textTab;

    ui->setupUi(this);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
}

void HighlightAddDialog::updateSettings() {
    settings = HighlightSettings::getInstance();
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

    if(!text.isEmpty()) {
        QColor color(textTab->bgColor.rgba()^0xffffff);
        HighlightSettingsEntry entry = HighlightSettingsEntry(0, text, group,
                                                              color, QColor(), false, "", false, 0, "", QBitArray(5));

        settings->addParameter("TextHighlight", entry);

        textTab->reloadHighlightList();

        ui->textLine->setText("");

        this->accept();
    }
}

void HighlightAddDialog::cancelPressed() {
    this->reject();
}

HighlightAddDialog::~HighlightAddDialog() {
    delete ui;
}
