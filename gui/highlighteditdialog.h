#ifndef HIGHLIGHTEDITDIALOG_H
#define HIGHLIGHTEDITDIALOG_H

#include <QDialog>
#include <QtGui>

#include <highlighttexttab.h>
#include <highlightsettings.h>
#include <highlightsettingsentry.h>


namespace Ui {
    class HighlightEditDialog;
}

class HighlightEditDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit HighlightEditDialog(HighlightTextTab *textTab, QWidget *parent = 0);
    ~HighlightEditDialog();

    QComboBox* getGroupSelect();

    void setEntry(HighlightSettingsEntry);

private:
    Ui::HighlightEditDialog *ui;
    HighlightTextTab *textTab;
    HighlightSettings *settings;
    HighlightSettingsEntry entry;

    QDialog* parenDialog;

    void showEvent(QShowEvent*);

private slots:
    void okPressed();
    void cancelPressed();
};

#endif // HIGHLIGHTEDITDIALOG_H
