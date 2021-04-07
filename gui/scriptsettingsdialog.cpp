#include "scriptsettingsdialog.h"
#include "ui_scriptsettingsdialog.h"

#include "mainwindow.h"
#include "clientsettings.h"

ScriptSettingsDialog::ScriptSettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ScriptSettingsDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)parent;

    settings = ClientSettings::getInstance();

    ui->scriptInterpreterInput->setObjectName("interpreterPath");
    ui->scriptEntryInput->setObjectName("scriptEntry");
    ui->scriptPathInput->setObjectName("scriptPath");
    ui->scriptExtensionInput->setObjectName("fileExtension");
    ui->scriptPortInput->setObjectName("apiPort");
    ui->lichLocationInput->setObjectName("lichLocation");
    ui->lichRubyInput->setObjectName("lichRuby");
    ui->lichArgumentsInput->setObjectName("lichArguments");

    ui->scriptInterpreterButton->setProperty("input", QVariant::fromValue<QLineEdit*>(ui->scriptInterpreterInput));
    ui->scriptEntryButton->setProperty("input", QVariant::fromValue<QLineEdit*>(ui->scriptEntryInput));
    ui->scriptPathButton->setProperty("input", QVariant::fromValue<QLineEdit*>(ui->scriptPathInput));
    ui->lichRubyButton->setProperty("input", QVariant::fromValue<QLineEdit*>(ui->lichRubyInput));
    ui->lichLocationButton->setProperty("input", QVariant::fromValue<QLineEdit*>(ui->lichLocationInput));

    ui->scriptPortInput->setValidator(new QRegExpValidator(QRegExp("^(\\s*|\\d+)$"), this));

    ui->applyButton->setDisabled(true);

    this->loadSettings();

    connect(ui->scriptInterpreterButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::browse);
    connect(ui->scriptEntryButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::browse);
    connect(ui->scriptPathButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::browse);

    connect(ui->lichRubyButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::browse);
    connect(ui->lichLocationButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::browse);

    QList<QLineEdit*> inputs;
    inputs << ui->scriptInterpreterInput << ui->scriptEntryInput << ui->scriptPathInput << ui->scriptExtensionInput << ui->scriptPortInput
            << ui->lichRubyInput << ui->lichLocationInput << ui->lichArgumentsInput;

    foreach(QLineEdit* input, inputs) {
        connect(input, &QLineEdit::editingFinished, this, &ScriptSettingsDialog::inputChanged);
        connect(input, &QLineEdit::textEdited, this, &ScriptSettingsDialog::inputEdited);
    }

    connect(ui->okButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::okPressed);
    connect(ui->applyButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::applyPressed);
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &ScriptSettingsDialog::cancelPressed);
}

void ScriptSettingsDialog::loadSettings() {
    ui->scriptInterpreterInput->setText(settings->getParameter("Script/interpreterPath", "").toString());
    ui->scriptEntryInput->setText(settings->getParameter("Script/scriptEntry", "").toString());
    ui->scriptPathInput->setText(settings->getParameter("Script/scriptPath", "").toString());
    ui->scriptExtensionInput->setText(settings->getParameter("Script/fileExtension", "").toString());
    ui->scriptPortInput->setText(settings->getParameter("Script/apiPort", "").toString());
    ui->lichRubyInput->setText(settings->getParameter("Script/lichRuby", "").toString());
    ui->lichLocationInput->setText(settings->getParameter("Script/lichLocation", "").toString());
    ui->lichArgumentsInput->setText(settings->getParameter("Script/lichArguments", "").toString());
}

void ScriptSettingsDialog::browse() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button != NULL) {
        QLineEdit* input = button->property("input").value<QLineEdit*>();
        QString directory = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Find Files"),
                settings->getParameter("Script/" + input->objectName(), QDir::currentPath()).toString()));
        if(!directory.isEmpty()) {
                input->setText(directory);
                changeList.insert("Script/" + input->objectName(), QVariant(directory));
                ui->applyButton->setEnabled(true);
        }
    }
}

void ScriptSettingsDialog::inputChanged() {
    QLineEdit* input = qobject_cast<QLineEdit*>(sender());
    if(input != NULL) {
        changeList.insert("Script/" + input->objectName(), QVariant(input->text()));
        ui->applyButton->setEnabled(true);
    }
}

void ScriptSettingsDialog::inputEdited(QString) {
    ui->applyButton->setEnabled(true);
}

void ScriptSettingsDialog::saveChanges() {
    QHashIterator<QString, QVariant> i(changeList);
    while (i.hasNext()) {
        i.next();
        settings->setParameter(i.key(), i.value());
    }
    changeList.clear();
    ui->applyButton->setDisabled(true);
    emit settingsChanged();
}

void ScriptSettingsDialog::cancelChanges() {
    if(!changeList.isEmpty()) {
        this->loadSettings();
        changeList.clear();
        ui->applyButton->setDisabled(true);
    }
}

void ScriptSettingsDialog::okPressed() {
    this->saveChanges();
    ui->applyButton->setDisabled(true);
    this->accept();
}

void ScriptSettingsDialog::applyPressed() {
    this->saveChanges();
    ui->applyButton->setDisabled(true);
}

void ScriptSettingsDialog::cancelPressed() {
    this->cancelChanges();
    this->reject();
}

ScriptSettingsDialog::~ScriptSettingsDialog() {
    delete ui;
}
