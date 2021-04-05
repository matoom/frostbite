#ifndef HIGHLIGHTEDITDIALOG_H
#define HIGHLIGHTEDITDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QComboBox>

#include "text/highlight/highlightsettingsentry.h"

class HighlightTextTab;
class HighlightSettings;

namespace Ui {
    class HighlightEditDialog;
}

class HighlightEditDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit HighlightEditDialog(HighlightTextTab *textTab, QWidget *parent = 0);
    ~HighlightEditDialog();

    QComboBox* getGroupSelect();

    void updateSettings();
    void setEntry(HighlightSettingsEntry);

private:
    Ui::HighlightEditDialog *ui;
    HighlightTextTab *textTab;
    HighlightSettings *settings;
    HighlightSettingsEntry entry;

    QDialog* parentDialog;

    void showEvent(QShowEvent*);

private slots:
    void okPressed();
    void cancelPressed();
};

#endif // HIGHLIGHTEDITDIALOG_H
