#include "abstracttabletab.h"

#include "defaultvalues.h"
#include "globaldefines.h"

AbstractTableTab::AbstractTableTab() {

}

bool AbstractTableTab::hasAny(QMap<int, QList<TableChangeEvent>> changeList, TableChangeEvent event) {
    for(int id : changeList.keys()) {
        for(TableChangeEvent e : changeList.value(id)) {
            if(e == event) return true;
        }
    }
    return false;
}

QMap<int, QList<TableChangeEvent>>& AbstractTableTab::getChangeEvents() {
    return this->changeEvents;
}

void AbstractTableTab::setChangeEvents(QMap<int, QList<TableChangeEvent>> changeEvents) {
   this->changeEvents = changeEvents;
}

QList<AlterSettingsEntry>& AbstractTableTab::getSettingEntries() {
    return this->settingEntries;
}

void AbstractTableTab::setSettingEntries(QList<AlterSettingsEntry> settingEntries) {
    this->settingEntries = settingEntries;
}

void AbstractTableTab::updateEntry(QTableWidgetItem* item) {
    int row = item->row();

    AlterSettingsEntry entry = settingEntries.at(row);

    QString data = item->data(Qt::UserRole).toString();

    if(data == "pattern") {
        entry.pattern = item->text();
    } else if(data == "value") {
        entry.value = item->text();
    }

    if(QRegularExpression(entry.pattern).isValid()) {
        item->setBackgroundColor(QColor(Qt::transparent));
    } else {
        item->setBackgroundColor(QColor(REGEX_ERROR_COLOR_HEX));
    }

    settingEntries.replace(row, entry);
    this->registerChange(row, TableChangeEvent::Update);
}

void AbstractTableTab::addNewTableRow(const QStringList& targetList) {
    getTable()->blockSignals(true);

    int count = getTable()->rowCount();
    getTable()->insertRow(count);

    AlterSettingsEntry entry(count, true, "", targetList);
    settingEntries.insert(count, entry);

    this->populateTableRow(count, entry);

    this->registerChange(count, TableChangeEvent::Add);

    getTable()->blockSignals(false);
}

void AbstractTableTab::registerChange(int row, TableChangeEvent event) {
    QList<TableChangeEvent> events;
    if(changeEvents.contains(row)) {
       events = changeEvents.value(row);
       if(!events.contains(event)) {
            events << event;
       }
    } else {
       events << event;
    }
    getApplyButton()->setDisabled(false);
    changeEvents.insert(row, events);
}

void AbstractTableTab::removeTableRow() {
    getTable()->blockSignals(true);

    int row = getTable()->currentRow();
    if(row >= 0) {
        getTable()->removeRow(row);

        settingEntries.removeAt(row);
        this->registerChange(row, TableChangeEvent::Remove);
    }
    getTable()->blockSignals(false);
}

void AbstractTableTab::displayMenu(QPoint pos) {
    QMenu menu(getTable());

    QList<QDockWidget*> dockWindows = this->getDockWindows();
    QModelIndexList selection = getTable()->selectionModel()->selectedRows();
    if (selection.count() > 0) {
        QModelIndex index = selection.at(0);
        AlterSettingsEntry entry = settingEntries.at(index.row());
        QStringList selected = entry.targetList;

        bool all = false;
        if(selected.contains(WINDOW_SELECT_ALL) || selected.empty()) all = true;

        QAction* allAction = menu.addAction(WINDOW_SELECT_ALL);
        allAction->setCheckable(true);
        allAction->setChecked(all);        

        QAction* mainAction = menu.addAction(WINDOW_TITLE_MAIN);
        mainAction->setCheckable(true);
        mainAction->setChecked(!all && selected.contains(WINDOW_TITLE_MAIN));

        for(QDockWidget* dock : dockWindows) {
            QAction *action = menu.addAction(dock->objectName());
            action->setCheckable(true);
            action->setChecked(!all && selected.contains(dock->objectName()));
        }

        menu.addSeparator();

        QAction* enabled = menu.addAction("Enabled");
        enabled->setCheckable(true);
        enabled->setChecked(entry.enabled);

        pos.rx()--; pos.ry()--;
        QAction *a = menu.exec(getTable()->viewport()->mapToGlobal(pos));
        if(a != NULL) {
            if(a->text() == "Enabled") {
                entry.enabled = a->isChecked();
            } else if(a->isChecked()) {
                entry.targetList.append(a->text());                
            } else {
                entry.targetList.removeAll(a->text());
            }
            settingEntries.replace(index.row(), entry);
            this->registerChange(index.row(), TableChangeEvent::Update);
        }
    }
}
