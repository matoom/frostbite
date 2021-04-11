#ifndef ALTERDIALOG_H
#define ALTERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QCheckBox>

#include <mainwindow.h>
#include <text/alter/substitutetab.h>
#include <text/alter/ignoretab.h>
#include <text/alter/linkstab.h>

namespace Ui {
    class AlterDialog;
}

class SubstituteTab;
class IgnoreTab;
class LinksTab;

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

    QTableWidget* getLinksTable();
    QPushButton* getLinksAddButton();
    QPushButton* getLinksRemoveButton();

    QList<QDockWidget*> getDockWindows();

    QPushButton* getApplyButton();

private:    
    Ui::AlterDialog *ui;

    MainWindow* mainWindow;
    SubstituteTab* substituteTab;
    IgnoreTab* ignoreTab;
    LinksTab* linksTab;

public slots:
    void applyPressed();
    void okPressed();
    void cancelPressed();
    void reloadSettings();
};

#endif // ALTERDIALOG_H
