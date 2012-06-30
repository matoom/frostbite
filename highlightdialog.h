#ifndef HIGHLIGHTDIALOG_H
#define HIGHLIGHTDIALOG_H

#include <QDialog>
#include <QColorDialog>
#include <QDebug>

#include <mainwindow.h>
#include <clientsettings.h>

#include <highlightgeneraltab.h>
#include <highlighttexttab.h>

class MainWindow;
class ClientSettings;
class HighlightGeneralTab;
class HighlightTextTab;

namespace Ui {
    class HighlightDialog;
}

class HighlightDialog : public QDialog {
    Q_OBJECT

public:
    explicit HighlightDialog(QWidget *parent = 0);
    ~HighlightDialog();

    /* general tab items */
    QListWidget* getGeneralList();
    //QLineEdit* getGeneralDelayLine();
    QGroupBox* getGeneralAlertGroup();
    QComboBox* getGeneralFileSelect();

    /* text tab items */
    QPushButton* getTextAddButton();
    QListWidget *getTextList();
    QComboBox *getTextFileSelect();
    //QLineEdit* getTextDelayLine();
    QGroupBox* getTextAlertGroup();

    /* highlight dialog items */
    QPushButton* getApplyButton();

private:
    Ui::HighlightDialog *ui;

    MainWindow *mainWindow;
    ClientSettings *settings;
    HighlightGeneralTab *generalTab;
    HighlightTextTab *textTab;

private slots:
    void okPressed();
    void cancelPressed();
    void applyPressed();
};

#endif // HIGHLIGHTDIALOG_H
