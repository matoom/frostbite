#ifndef MACRODIALOG_H
#define MACRODIALOG_H

#include <QDialog>
#include <QDebug>
#include <QTableWidget>
#include <QKeyEvent>

#include <macrosettings.h>

class MacroSettings;

namespace Ui {
    class MacroDialog;
}

class MacroDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit MacroDialog(QWidget *parent = 0);
    ~MacroDialog();

    void updateSettings();
    void loadSettings();
    void loadSequenceTime();
    
private:
    Ui::MacroDialog *ui;
    MacroSettings* macroSettings;    
    QHash<QString, QHash<int, QTableWidgetItem*> > changeList;
    QHash<QString, QList<QKeyEvent> > keys;

    void buildKeys(QString tabName, Qt::KeyboardModifiers modifers, int start, int end);
    void loadKeys(QString tabName, QTableWidget* table);

    void buildKeypadKeys(Qt::KeyboardModifiers modifers);
    void buildKeypadKeys(Qt::KeyboardModifiers modifers, int start, int end);

    void saveSequenceTime();
    void cancelSequenceTime();

    bool saveTime;

private slots:
    void altCommandChanged(QTableWidgetItem*, QTableWidgetItem*);
    void ctrlCommandChanged(QTableWidgetItem*, QTableWidgetItem*);
    void functionCommandChanged(QTableWidgetItem*, QTableWidgetItem*);
    void keypadCommandChanged(QTableWidgetItem*, QTableWidgetItem*);
    void sequenceTimeChanged(const QString&);
    void saveChanges();
    void cancelChanges();
    void okPressed();
    void applyPressed();
    void cancelPressed();
};

#endif // MACRODIALOG_H
