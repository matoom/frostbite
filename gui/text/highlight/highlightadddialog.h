#ifndef HIGHLIGHTADDDIALOG_H
#define HIGHLIGHTADDDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QComboBox>
#include <QGroupBox>
#include <QListWidget>
#include <QListWidgetItem>

class HighlightTextTab;
class HighlightSettings;

namespace Ui {
    class HighlightAddDialog;
}

class HighlightAddDialog : public QDialog {
    Q_OBJECT

public:
    explicit HighlightAddDialog(HighlightTextTab *textTab, QWidget *parent = 0);
    ~HighlightAddDialog();

    QComboBox* getGroupSelect();
    void updateSettings();

private:
    Ui::HighlightAddDialog* ui;
    HighlightTextTab* textTab;
    HighlightSettings* settings;
    QDialog* parentDialog;

    void showEvent(QShowEvent* event);

private slots:
    void okPressed();
    void cancelPressed();
};

#endif // HIGHLIGHTADDDIALOG_H
