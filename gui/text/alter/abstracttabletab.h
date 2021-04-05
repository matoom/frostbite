#ifndef ABSTRACTTABLETAB_H
#define ABSTRACTTABLETAB_H

#include <QMap>
#include <QTableWidget>
#include <QPushButton>
#include <QDockWidget>
#include <QMenu>

#include "text/alter/altersettingsentry.h"

enum class TableChangeEvent { Add, Remove, Update };

class AbstractTableTab {

public:
    AbstractTableTab();

    virtual void populateTableRow(int, AlterSettingsEntry) = 0;
    virtual QTableWidget* getTable() = 0;
    virtual QPushButton* getApplyButton() = 0;
    virtual QList<QDockWidget*> getDockWindows() = 0;
    virtual void print(QString) = 0;

    void registerChange(int, TableChangeEvent);

    bool hasAny(QMap<int, QList<TableChangeEvent>> changeList, TableChangeEvent event);

    QMap<int, QList<TableChangeEvent>>& getChangeEvents();
    void setChangeEvents(QMap<int, QList<TableChangeEvent>> changeEvents);

    QList<AlterSettingsEntry>& getSettingEntries();
    void setSettingEntries(QList<AlterSettingsEntry> settingEntries);

    void updateEntry(QTableWidgetItem* item);
    void addNewTableRow();
    void removeTableRow();
    void displayMenu(QPoint pos);

private:
    QMap<int, QList<TableChangeEvent>> changeEvents;
    QList<AlterSettingsEntry> settingEntries;

};

#endif // ABSTRACTTABLETAB_H
