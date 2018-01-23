#include "appearancedialog.h"
#include "ui_appearancedialog.h"

AppearanceDialog::AppearanceDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AppearanceDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)parent;
    windowFacade = mainWindow->getWindowFacade();
    commandLine = mainWindow->getCommandLine();
    roundTimeDisplay = commandLine->getRoundtimeDisplay();

    settings = GeneralSettings::getInstance();

    this->populateMainBox();
    this->populateDockBox();
    this->populateCmdBox();
    this->loadSettings();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void AppearanceDialog::reloadSettings() {
    settings = GeneralSettings::getInstance();
    this->loadSettings();
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

    cmdFontValue = settings->cmdFont();
    this->setSelectFont("Commandline/font", cmdFontSelect, QFont(DEFAULT_CMD_FONT, DEFAULT_CMD_FONT_SIZE));

    cmdBackgroundValue = settings->cmdBackground();
    this->setSelectBackground("Commandline/background", cmdBgSelect, DEFAULT_CMD_BACKGROUND);

    cmdFontColorValue = settings->cmdFontColor();
    this->setSelectBackground("Commandline/fontColor", cmdFontColorSelect, DEFAULT_CMD_FONT_COLOR);

    cmdRtColorValue = settings->cmdRtColor();
    this->setSelectBackground("Commandline/rtColor", cmdRtColorSelect, DEFAULT_CMD_RT_COLOR);

    cmdCtColorValue = settings->cmdCtColor();
    this->setSelectBackground("Commandline/ctColor", cmdCtColorSelect, DEFAULT_CMD_CT_COLOR);
}

QToolButton* AppearanceDialog::selectButton(int width, int height, QString toolTip) {
    QToolButton* button = new QToolButton(this);
    button->setToolTip(toolTip);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setMenu(new QMenu(button));
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

void AppearanceDialog::populateCmdBox() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(label("Background:"));

    cmdBgSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(cmdBgSelect, SIGNAL(clicked()), this, SLOT(selectCmdBg()));
    hLayout->addWidget(cmdBgSelect);

    ui->cmdVLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    hLayout->addWidget(label("Round time:"));

    cmdRtColorSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(cmdRtColorSelect, SIGNAL(clicked()), this, SLOT(selectCmdRtColor()));
    hLayout->addWidget(cmdRtColorSelect);

    ui->cmdVLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    hLayout->addWidget(label("Cast time:"));

    cmdCtColorSelect = selectButton(40, 22, "Click here to pick a new color.");
    connect(cmdCtColorSelect, SIGNAL(clicked()), this, SLOT(selectCmdCtColor()));
    hLayout->addWidget(cmdCtColorSelect);

    ui->cmdVLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    hLayout->addWidget(label("Text:"));

    cmdFontSelect = selectButton(200, 22, "Click here to pick a new font.");
    connect(cmdFontSelect, SIGNAL(clicked()), this, SLOT(selectCmdFont()));
    hLayout->addWidget(cmdFontSelect);

    cmdFontColorSelect = selectButton(40, 20, "Click here to pick a new color.");
    connect(cmdFontColorSelect, SIGNAL(clicked()), this, SLOT(selectCmdFontColor()));
    hLayout->addWidget(cmdFontColorSelect);

    ui->cmdVLayout->addLayout(hLayout);
}

void AppearanceDialog::reset() {    
    windowFacade->setGameWindowBackground(mainBackgroundValue);
    windowFacade->setGameWindowFont(mainFontValue);
    windowFacade->setGameWindowFontColor(mainFontColorValue);

    windowFacade->setDockBackground(dockBackgroundValue);
    windowFacade->setDockFont(dockFontValue);
    windowFacade->setDockFontColor(dockFontColorValue);

    commandLine->setCmdBgColor(cmdBackgroundValue);
    commandLine->setCmdFont(cmdFontValue);
    commandLine->setCmdFontColor(cmdFontColorValue);

    roundTimeDisplay->setRtColor(cmdRtColorValue);
    roundTimeDisplay->setCtColor(cmdCtColorValue);
}

void AppearanceDialog::selectMainBg() {
    QColor selectedColor = QColorDialog::getColor(mainBackgroundValue, this);
    if(selectedColor.isValid()) {
        mainBgSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                      "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        windowFacade->setGameWindowBackground(selectedColor);
        mainBackgroundValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("GameWindow/background", selectedColor);
    }
}

void AppearanceDialog::selectMainFont() {
    bool ok;
    mainFontValue = QFontDialog::getFont(&ok, mainFontValue, this);

    if(ok) {
        mainFontSelect->setText(mainFontValue.family() + "," + QString::number(mainFontValue.pointSize()));
        windowFacade->setGameWindowFont(mainFontValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("GameWindow/font", mainFontValue);
    }
}

void AppearanceDialog::selectMainFontColor() {
    QColor selectedColor = QColorDialog::getColor(mainFontColorValue, this);
    if(selectedColor.isValid()) {
        mainFontColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        windowFacade->setGameWindowFontColor(selectedColor);
        mainFontColorValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("GameWindow/fontColor", selectedColor);
    }
}

void AppearanceDialog::selectDockBg() {
    QColor selectedColor = QColorDialog::getColor(dockBackgroundValue, this);
    if(selectedColor.isValid()) {
        dockBgSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                      "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        windowFacade->setDockBackground(selectedColor);
        dockBackgroundValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("DockWindow/background", selectedColor);
    }
}

void AppearanceDialog::selectDockFont() {
    bool ok;
    dockFontValue = QFontDialog::getFont(&ok, dockFontValue, this);

    if(ok) {
        dockFontSelect->setText(dockFontValue.family() + "," + QString::number(dockFontValue.pointSize()));
        windowFacade->setDockFont(dockFontValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("DockWindow/font", dockFontValue);
    }
}

void AppearanceDialog::selectDockFontColor() {
    QColor selectedColor = QColorDialog::getColor(dockFontColorValue, this);
    if(selectedColor.isValid()) {
        dockFontColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        windowFacade->setDockFontColor(selectedColor);
        dockFontColorValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("DockWindow/fontColor", selectedColor);
    }
}

void AppearanceDialog::selectCmdBg() {
    QColor selectedColor = QColorDialog::getColor(cmdBackgroundValue, this);
    if(selectedColor.isValid()) {
        cmdBgSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                      "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        commandLine->setCmdBgColor(selectedColor);
        cmdBackgroundValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("Commandline/background", selectedColor);
    }
}

void AppearanceDialog::selectCmdFont() {
    bool ok;
    cmdFontValue = QFontDialog::getFont(&ok, cmdFontValue, this);

    if(ok) {
        cmdFontSelect->setText(cmdFontValue.family() + "," + QString::number(cmdFontValue.pointSize()));
        commandLine->setCmdFont(cmdFontValue);

        ui->applyButton->setEnabled(true);
        changeList.insert("Commandline/font", cmdFontValue);
    }
}

void AppearanceDialog::selectCmdFontColor() {
    QColor selectedColor = QColorDialog::getColor(cmdFontColorValue, this);
    if(selectedColor.isValid()) {
        cmdFontColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        commandLine->setCmdFontColor(selectedColor);
        cmdFontColorValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("Commandline/fontColor", selectedColor);
    }
}

void AppearanceDialog::selectCmdRtColor() {
    QColor selectedColor = QColorDialog::getColor(cmdRtColorValue, this);
    if(selectedColor.isValid()) {
        cmdRtColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        roundTimeDisplay->setRtColor(selectedColor);
        cmdRtColorValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("Commandline/rtColor", selectedColor);
    }
}

void AppearanceDialog::selectCmdCtColor() {
    QColor selectedColor = QColorDialog::getColor(cmdCtColorValue, this);
    if(selectedColor.isValid()) {
        cmdCtColorSelect->setStyleSheet(QString("QToolButton { background: %1;"
                                             "border: 1px solid #C0C0C0; }").arg(selectedColor.name()));
        roundTimeDisplay->setCtColor(selectedColor);
        cmdCtColorValue = selectedColor;

        ui->applyButton->setEnabled(true);
        changeList.insert("Commandline/ctColor", selectedColor);
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
    delete cmdBgSelect;
    delete cmdFontSelect;
    delete cmdFontColorSelect;
    delete cmdRtColorSelect;
    delete cmdCtColorSelect;
    delete ui;
}
