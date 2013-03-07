#include "scripteditdialog.h"
#include "ui_scripteditdialog.h"

ScriptEditDialog::ScriptEditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ScriptEditDialog) {
    ui->setupUi(this);

    completer = new QCompleter(this);
    model = new QStringListModel();

    scriptPath.setPath(QDir::currentPath() + "/scripts");
    notepadPath.setPath(QDir::currentPath() + "/notepad/Notepad2.exe");

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void ScriptEditDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    QStringList filter;
    filter << "*.rb";

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
