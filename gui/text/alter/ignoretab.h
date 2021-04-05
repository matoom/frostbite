#ifndef IGNORETAB_H
#define IGNORETAB_H

#include <QObject>
#include <QCheckBox>

#include "text/alter/abstracttabletab.h"

class AlterDialog;
class AlterSettingsEntry;
class IgnoreSettings;

enum class TableChangeEvent;

class IgnoreTab : public QObject, public AbstractTableTab {
    Q_OBJECT

public:
    explicit IgnoreTab(QObject *parent = 0);
    ~IgnoreTab();

    void updateSettings();
    void saveChanges();
    void cancelChanges();

    void print(QString text);

    QTableWidget* getTable();
    QPushButton* getApplyButton();
    QList<QDockWidget*> getDockWindows();

private:
    AlterDialog* alterDialog;

    QTableWidget* ignoreTable;
    QPushButton* addButton;
    QPushButton* removeButton;
    QCheckBox* ignoreEnabled;

    IgnoreSettings* settings;

    void initIgnoreList();
    void populateTableRow(int row, AlterSettingsEntry entry);

signals:

public slots:
    void addNewTableRow();
    void removeTableRow();
    void displayMenu(QPoint);
    void updateEntry(QTableWidgetItem* item);
    void enabledChanged(int);
};

#endif // IGNORETAB_H
