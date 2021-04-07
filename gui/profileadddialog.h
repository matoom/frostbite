#ifndef PROFILEADDDIALOG_H
#define PROFILEADDDIALOG_H

#include <QDialog>
#include <QDir>
#include <QStandardPaths>

class ClientSettings;

namespace Ui {
    class ProfileAddDialog;
}

class ProfileAddDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ProfileAddDialog(QWidget *parent = 0);
    ~ProfileAddDialog();
    
private:
    Ui::ProfileAddDialog *ui;
    ClientSettings* clientSettings;

    void createProfile();
    void populate();

private slots:
    void okClicked();
    void cancelClicked();

signals:
    void updateMenu();
};

#endif // PROFILEADDDIALOG_H
