#ifndef ABSTRACTTABLETAB_H
#define ABSTRACTTABLETAB_H

#include <QHash>
#include <QTableWidget>
#include <QPushButton>
#include <QDockWidget>
#include <QMenu>

#include <defaultvalues.h>

#include <text/alter/altersettingsentry.h>

#define WINDOW_SELECT_ALL "All"

enum class TableChangeEvent { Add, Remove, Update };

class AbstractTableTab {

public:
    AbstractTableTab();

    virtual void populateTableRow(int, AlterSettingsEntry) = 0;
    virtual QTableWidget* getTable() = 0;
    virtual QPushButton* getApplyButton() = 0;
    virtual QList<QDockWidget*> getDockWindows() = 0;

    void registerChange(int, TableChangeEvent);

    bool hasAny(QHash<int, QList<TableChangeEvent>> changeList, TableChangeEvent event);

    QHash<int, QList<TableChangeEvent>>& getChangeEvents();
    void setChangeEvents(QHash<int, QList<TableChangeEvent>> changeEvents);

    QList<AlterSettingsEntry>& getSettingEntries();
    void setSettingEntries(QList<AlterSettingsEntry> settingEntries);

    void updateEntry(QTableWidgetItem* item);
    void addNewTableRow();
    void removeTableRow();
    void displayMenu(QPoint pos);

private:
    QHash<int, QList<TableChangeEvent>> changeEvents;
    QList<AlterSettingsEntry> settingEntries;

};

#endif // ABSTRACTTABLETAB_H
