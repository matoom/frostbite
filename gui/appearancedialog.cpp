#include "appearancedialog.h"
#include "ui_appearancedialog.h"

AppearanceDialog::AppearanceDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AppearanceDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)parent;
    windowManager = mainWindow->getWindowManager();
    settings = new GeneralSettings();

    this->populateMainBox();
    this->populateDockBox();
    this->loadSettings();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void AppearanceDialog::updateSettings() {
    settings->init();
}

void AppearanceDialog::loadSettings() {
    mainFontValue = settings->gameWindowFont();
    this->setSelectFont("GameWindow/font", mainFontSelect, QFont(DEFAULT_MAIN_FONT, DEFAULT_MAIN_FONT_SIZE));

    mainBackgroundValue = settings->gameWindowBackground();
    this->setSelectBackground("GameWindow/background", mainBgSelect, DEFAULT_MAIN_BACKGROUND);

    mainFontColorValue = settings->gameWindowFontColor();
    this->setSelectBackground("GameWindow/fontColor", mainFontColorSelect, DEFAULT_MAIN_FONT_COLOR);

    dockFontValue = settings->dockWindowFont();
    this->setSelectFont("DockWindow/font", dockFontSelect, QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE));

    dockBackgroundValue = settings->dockWindowBackground();
    this->setSelectBackground("DockWindow/background", dockBgSelect, DEFAULT_DOCK_BACKGROUND);

    dockFontColorValue = settings->dockWindowFontColor();
    this->setSelectBackground("DockWindow/fontColor", dockFontColorSelect, DEFAULT_DOCK_FONT_COLOR);
}

QToolButton* AppearanceDialog::selectButton(int width, int height, QString toolTip) {
    QToolButton* button = new QToolButton;
    button->setToolTip(toolTip);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setMenu(new QMenu());
    button->setMinimumWidth(width);
    button->setMinimumHeight(height);

    return button;
}

QLabel* AppearanceDialog::label(QString text) {
    QLabel* label = new QLabel(this);
    label->setText(text);

    return label;
}

void AppearanceDialog::setSelectBackground(QString name, QToolButton* button, QString defaultColor) {
    QColor color = settings->getParameter(name, defaultColor).value<QColor>();
    button->setStyleSheet(QString("QToolButton { background: %1;"
                                  "border: 1px solid #C0C0C0; }").arg(color.name()));
}

void AppearanceDialog::setSelectFont(QString name, QToolButton* button, QFont defaultFont) {
    QFont font = settings->getParameter(name, defaultFont).value<QFont>();
    button->setText(font.family() + "," + QString::number(font.pointSize()));
}

void AppearanceDialog::populateMainBox() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(label("Background:"));

    mainBgSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(mainBgSelect, SIGNAL(clicked()), this, SLOT(selectMainBg()));
    hLayout->addWidget(mainBgSelect);

    ui->mainVLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();

    hLayout->addWidget(label("Text:"));

    mainFontSelect = selectButton(200, 22, "Click here to pick a new font.");
    connect(mainFontSelect, SIGNAL(clicked()), this, SLOT(selectMainFont()));
    hLayout->addWidget(mainFontSelect);

    mainFontColorSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(mainFontColorSelect, SIGNAL(clicked()), this, SLOT(selectMainFontColor()));
    hLayout->addWidget(mainFontColorSelect);

    ui->mainVLayout->addLayout(hLayout);    
}

void AppearanceDialog::populateDockBox() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(label("Background:"));

    dockBgSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(dockBgSelect, SIGNAL(clicked()), this, SLOT(selectDockBg()));
    hLayout->addWidget(dockBgSelect);

    ui->dockVLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();

    hLayout->addWidget(label("Text:"));

    dockFontSelect = selectButton(200, 22, "Click here to pick a new font.");
    connect(dockFontSelect, SIGNAL(clicked()), this, SLOT(selectDockFont()));
    hLayout->addWidget(dockFontSelect);

    dockFontColorSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(dockFontColorSelect, SIGNAL(clicked()), this, SLOT(selectDockFontColor()));
    hLayout->addWidget(dockFontColorSelect);

    ui->dockVLayout->addLayout(hLayout);
}

void AppearanceDialog::reset() {
    mainWindow->setBackgroundColor(mainBackgroundValue);
    windowManager->setGameWindowFont(mainFontValue);
    windowManager->setGameWindowFontColor(mainFontColorValue);

    windowManager->setDockBackground(dockBackgroundValue);
    windowManager->setDockFont(dockFontValue);
    windowManager->setDockFontColor(dockFontColorValue);
}

void AppearanceDialog::selectMainBg() {
    mainBackgroundValue = QColorDialog::getColor(mainBackgroundValue, this);
    if(mainBackgroundValue.isValid()) {
        mainBgSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                      "border: 1px solid #C0C0C0; }").arg(mainBackgroundValue.name()));
        mainWindow->setBackgroundColor(mainBackgroundValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("GameWindow/background", mainBackgroundValue);
    }
}

void AppearanceDialog::selectMainFont() {
    bool ok;
    mainFontValue = QFontDialog::getFont(&ok, mainFontValue, this);

    if(ok) {
        mainFontSelect->setText(mainFontValue.family() + "," + QString::number(mainFontValue.pointSize()));
        windowManager->setGameWindowFont(mainFontValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("GameWindow/font", mainFontValue);
    }
}

void AppearanceDialog::selectMainFontColor() {
    mainFontColorValue = QColorDialog::getColor(mainFontColorValue, this);
    if(mainFontColorValue.isValid()) {
        mainFontColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(mainFontColorValue.name()));
        windowManager->setGameWindowFontColor(mainFontColorValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("GameWindow/fontColor", mainFontColorValue);
    }
}

void AppearanceDialog::selectDockBg() {
    dockBackgroundValue = QColorDialog::getColor(dockBackgroundValue, this);
    if(dockBackgroundValue.isValid()) {
        dockBgSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                      "border: 1px solid #C0C0C0; }").arg(dockBackgroundValue.name()));
        windowManager->setDockBackground(dockBackgroundValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("DockWindow/background", dockBackgroundValue);
    }
}

void AppearanceDialog::selectDockFont() {
    bool ok;
    dockFontValue = QFontDialog::getFont(&ok, dockFontValue, this);

    if(ok) {
        dockFontSelect->setText(dockFontValue.family() + "," + QString::number(dockFontValue.pointSize()));
        windowManager->setDockFont(dockFontValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("DockWindow/font", dockFontValue);
    }
}

void AppearanceDialog::selectDockFontColor() {
    dockFontColorValue = QColorDialog::getColor(dockFontColorValue, this);
    if(dockFontColorValue.isValid()) {
        dockFontColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(dockFontColorValue.name()));
        windowManager->setDockFontColor(dockFontColorValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("DockWindow/fontColor", dockFontColorValue);
    }
}

void AppearanceDialog::saveChanges() {
    QHashIterator<QString, QVariant> i(changeList);
    while (i.hasNext()) {
        i.next();
        settings->setParameter(i.key(), i.value());
    }
    changeList.clear();
}

void AppearanceDialog::cancelChanges() {
    if(!changeList.isEmpty()) {
        this->loadSettings();
        this->reset();
        changeList.clear();
        ui->applyButton->setDisabled(true);
    }
}

void AppearanceDialog::okPressed() {
    this->saveChanges();

    ui->applyButton->setDisabled(true);

    this->accept();
}

void AppearanceDialog::applyPressed() {
    this->saveChanges();

    ui->applyButton->setDisabled(true);
}

void AppearanceDialog::cancelPressed() {
    this->cancelChanges();

    this->reject();
}

AppearanceDialog::~AppearanceDialog() {
    delete mainBgSelect;
    delete mainFontSelect;
    delete mainFontColorSelect;
    delete dockBgSelect;
    delete dockFontSelect;
    delete dockFontColorSelect;
    delete ui;
}
