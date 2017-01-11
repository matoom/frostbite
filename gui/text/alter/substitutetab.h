#ifndef SUBSTITUTETAB_H
#define SUBSTITUTETAB_H

#include <QObject>

#include <text/alter/alterdialog.h>
#include <text/alter/altersettingsentry.h>
#include <text/alter/substitutionsettings.h>
#include <text/alter/abstracttabletab.h>

class AlterDialog;

class SubstituteTab : public QObject, AbstractTableTab {
    Q_OBJECT

public:
    explicit SubstituteTab(QObject *parent = 0);

    void updateSettings();
    void saveChanges();
    void cancelChanges();

    void print(QString text);

    QTableWidget* getTable();
    QPushButton* getApplyButton();
    QList<QDockWidget*> getDockWindows();

private:
    AlterDialog* alterDialog;

    QTableWidget* substitutionTable;
    QPushButton* addButton;
    QPushButton* removeButton;

    SubstitutionSettings* settings;

    void initSubstitutionList();

    void populateTableRow(int row, AlterSettingsEntry entry);

signals:

public slots:
    void removeTableRow();
    void addNewTableRow();
    void displayMenu(QPoint);
    void updateEntry(QTableWidgetItem*);
};

#endif // SUBSTITUTETAB_H
