#ifndef SCRIPTEDITDIALOG_H
#define SCRIPTEDITDIALOG_H

#include <QDialog>
#include <QCompleter>
#include <QDir>
#include <QStringListModel>
#include <QProcess>
#include <QDebug>

#include <clientsettings.h>
#include <defaultvalues.h>

namespace Ui {
    class ScriptEditDialog;
}

class ScriptEditDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ScriptEditDialog(QWidget *parent = 0);
    ~ScriptEditDialog();
    
private:   
    Ui::ScriptEditDialog *ui;
    QCompleter* completer;
    QStringListModel* model;
    QDir scriptPath;
    QDir notepadPath;

    void showEvent(QShowEvent*);

private slots:
    void okPressed();
    void cancelPressed();

};

#endif // SCRIPTEDITDIALOG_H
