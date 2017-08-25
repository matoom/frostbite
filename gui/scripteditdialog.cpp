#include "scripteditdialog.h"
#include "ui_scripteditdialog.h"

ScriptEditDialog::ScriptEditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ScriptEditDialog) {
    ui->setupUi(this);

    completer = new QCompleter(this);
    model = new QStringListModel();

    scriptPath.setPath(ClientSettings::getInstance()->getQStringNotBlank("Script/scriptPath", SCRIPT_PATH));

    #ifdef Q_OS_WIN
        notepadPath.setPath("notepad");
    #elif defined(Q_OS_LINUX)
        notepadPath.setPath("gedit");
    #elif defined(Q_OS_MAC)
        notepadPath.setPath("open -t");
    #endif

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void ScriptEditDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    QStringList filter;
    filter << "*" + ClientSettings::getInstance()->getQStringNotBlank("Script/fileExtension", SCRIPT_FILE_EXTENSION);

    QStringList fileList = scriptPath.entryList(filter, QDir::Files, QDir::Name);

    model->setStringList(fileList);
    completer->setModel(model);

    ui->fileEdit->setCompleter(completer);

    ui->fileEdit->setFocus();
}

void ScriptEditDialog::okPressed() {
    this->accept();
    QProcess::startDetached(notepadPath.path() + " " +
        scriptPath.path() + "/" + ui->fileEdit->text());
}

void ScriptEditDialog::cancelPressed() {
    this->reject();
}

ScriptEditDialog::~ScriptEditDialog() {
    delete model;
    delete completer;
    delete ui;
}
