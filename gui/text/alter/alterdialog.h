#ifndef ALTERDIALOG_H
#define ALTERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QCheckBox>

#include <mainwindow.h>
#include <text/alter/substitutetab.h>
#include <text/alter/ignoretab.h>

namespace Ui {
    class AlterDialog;
}

class SubstituteTab;
class IgnoreTab;

class AlterDialog : public QDialog {
    Q_OBJECT

public:
    explicit AlterDialog(QWidget *parent = 0);
    ~AlterDialog();

    void updateSettings();

    QPushButton* getSubstitutionAddButton();
    QPushButton* getSubstitutionRemoveButton();
    QTableWidget* getSubstitutionTable();

    QPushButton* getIgnoreAddButton();
    QPushButton* getIgnoreRemoveButton();
    QTableWidget* getIgnoreTable();
    QCheckBox* getIgnoreEnabled();

    QList<QDockWidget*> getDockWindows();

    QPushButton* getApplyButton();

private:    
    Ui::AlterDialog *ui;

    MainWindow* mainWindow;
    SubstituteTab* substituteTab;
    IgnoreTab* ignoreTab;

public slots:
    void applyPressed();
    void okPressed();
    void cancelPressed();
    void reloadSettings();
};

#endif // ALTERDIALOG_H
