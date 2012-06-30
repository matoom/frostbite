#include "highlightdialog.h"
#include "ui_highlightdialog.h"

HighlightDialog::HighlightDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HighlightDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);
    settings = ClientSettings::Instance();

    generalTab = new HighlightGeneralTab(this);
    textTab = new HighlightTextTab(this);

    //http://www.qtcentre.org/threads/40725-QColorDialog
    //ui->colorButton->setPalette(QColorDialog::getColor(Qt::green));
    //this->defaultPalette();

    /*QHash<QString, QVariant> alert;
    QHash<QString, QVariant> item1;
    item1.insert("name", "[Room titles]");
    item1.insert("color", QColor(255, 255, 255, 255));    
    alert.insert("file", "chimes.wav");
    alert.insert("delay", 200);
    item1.insert("alert", alert);
    //item1.insert("flags", HighLight::EntireRow);
    generalHighlights.insert("RoomName", item1);

    item1.insert("name", "Game Messages");
    item1.insert("color", QColor(255, 255, 0, 255));
    item1.insert("alert", NULL);
    //item1.insert("flags", HighLight::SingleWord);
    generalHighlights.insert("GameMessage", item1);

    item1.insert("name", "Someone says ..");
    item1.insert("color", QColor(0, 255, 0, 255));
    alert.insert("file", "Windows Pop-up Blocked.wav");
    alert.insert("delay", 0);
    item1.insert("alert", alert);
    //item1.insert("flags", HighLight::PartialMatches | HighLight::EntireRow);
    generalHighlights.insert("Say" ,item1);

    item1.insert("name", "Also see..");
    item1.insert("color", QColor(0, 255, 255, 255));
    item1.insert("alert", NULL);
    //item1.insert("flags", HighLight::EntireRow);
    generalHighlights.insert("AlsoSee", item1);

    item1.insert("name", "Also here..");
    item1.insert("color", QColor(255, 255, 0, 255));
    item1.insert("alert", NULL);
    //item1.insert("flags", HighLight::EntireRow | HighLight::StartingWith);
    generalHighlights.insert("AlsoHere", item1);*/

    //this->loadSettings();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
}

QListWidget* HighlightDialog::getGeneralList() {
    return ui->gList;
}

/*QLineEdit* HighlightDialog::getGeneralDelayLine() {
    return ui->gDelay;
}*/

QGroupBox* HighlightDialog::getGeneralAlertGroup() {
    return ui->gAlertGroup;
}

QComboBox* HighlightDialog::getGeneralFileSelect() {
    return ui->gFileSelect;
}

QPushButton* HighlightDialog::getApplyButton() {
    return ui->applyButton;
}

QPushButton* HighlightDialog::getTextAddButton() {
    return ui->tAdd;
}

QListWidget* HighlightDialog::getTextList() {
    return ui->tList;
}

QComboBox* HighlightDialog::getTextFileSelect() {
    return ui->tFileSelect;
}

/*QLineEdit* HighlightDialog::getTextDelayLine(){
    return ui->tAlertDelay;
}*/

QGroupBox* HighlightDialog::getTextAlertGroup(){
    return ui->tAlertGroup;
}

void HighlightDialog::okPressed() {
    generalTab->saveChanges();

    ui->applyButton->setEnabled(false);

    this->accept();
}

void HighlightDialog::cancelPressed() {
    generalTab->cancelChanges();

    ui->applyButton->setEnabled(false);

    this->reject();
}

void HighlightDialog::applyPressed() {
    generalTab->saveChanges();

    ui->applyButton->setEnabled(false);
}

HighlightDialog::~HighlightDialog() {
    delete ui;
}
