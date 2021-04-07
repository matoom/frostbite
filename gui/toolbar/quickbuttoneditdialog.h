#ifndef QUICKBUTTONEDITDIALOG_H
#define QUICKBUTTONEDITDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QToolButton>


class GeneralSettings;

namespace Ui {
    class QuickButtonEditDialog;
}

class QuickButtonEditDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit QuickButtonEditDialog(QWidget *parent = 0);
    ~QuickButtonEditDialog();

    void updateButton(QToolButton*);
    void updateLocation(const QPoint&);

private:
    GeneralSettings* settings;
    QToolButton* button;

    void showEvent(QShowEvent*);

private slots:
    void okPressed();
    void cancelPressed();

private:
    Ui::QuickButtonEditDialog *ui;
};

#endif // QUICKBUTTONEDITDIALOG_H
