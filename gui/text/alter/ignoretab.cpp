#include "ignoretab.h"

IgnoreTab::IgnoreTab(QObject *parent) : QObject(parent), AbstractTableTab() {
    alterDialog = (AlterDialog*)parent;

    ignoreTable = alterDialog->getIgnoreTable();
    addButton = alterDialog->getIgnoreAddButton();
    removeButton = alterDialog->getIgnoreRemoveButton();

    settings = new IgnoreSettings();

    QStringList labels;
    labels << "Pattern";
    ignoreTable->setColumnCount(labels.count());
    ignoreTable->setHorizontalHeaderLabels(labels);

    ignoreTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    ignoreTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ignoreTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ignoreTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addNewTableRow()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeTableRow()));

    connect(ignoreTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayMenu(QPoint)));
    connect(ignoreTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateEntry(QTableWidgetItem*)));

    this->initIgnoreList();
}

void IgnoreTab::updateSettings() {
    delete settings;
    settings = new IgnoreSettings();
    this->initIgnoreList();
}

void IgnoreTab::addNewTableRow() {
   AbstractTableTab::addNewTableRow();
}

void IgnoreTab::removeTableRow() {
    AbstractTableTab::removeTableRow();
}

void IgnoreTab::updateEntry(QTableWidgetItem* item) {
   AbstractTableTab::updateEntry(item);
}

void IgnoreTab::displayMenu(QPoint pos) {
    AbstractTableTab::displayMenu(pos);
}

void IgnoreTab::initIgnoreList() {
    this->setSettingEntries(settings->getIgnores());

    ignoreTable->blockSignals(true);

    ignoreTable->clearContents();
    ignoreTable->setRowCount(this->getSettingEntries().size());

    for(int i = 0; i < this->getSettingEntries().size(); i++) {
        AlterSettingsEntry entry = getSettingEntries().at(i);
        this->populateTableRow(i, entry);
    }
    ignoreTable->blockSignals(false);
}

void IgnoreTab::populateTableRow(int row, AlterSettingsEntry entry) {
    QTableWidgetItem* patternItem = new QTableWidgetItem(entry.pattern);
    patternItem->setData(Qt::UserRole, "pattern");

    if(QRegExp(entry.pattern).isValid()) {
        patternItem->setBackgroundColor(QColor(Qt::transparent));
    } else {
        patternItem->setBackgroundColor(QColor("#EE3233"));
    }

    ignoreTable->setItem(row, 0, patternItem);
}

void IgnoreTab::saveChanges() {
    // rewrite all settings if any remove events
    if(this->hasAny(this->getChangeEvents(), TableChangeEvent::Remove)) {
        settings->setSettings(this->getSettingEntries());
    } else {
        for(int id : this->getChangeEvents().keys()) {
            AlterSettingsEntry entry = this->getSettingEntries().at(id);
            QListIterator<TableChangeEvent> it(this->getChangeEvents().value(id));
            it.toBack();
            while (it.hasPrevious()) {
                TableChangeEvent e = it.previous();
                if(e == TableChangeEvent::Add) {
                    settings->addParameter(entry);
                    break;
                } else if(e == TableChangeEvent::Update) {
                    settings->setParameter(entry);
                }
            }
        }
    }
    this->getChangeEvents().clear();
}

void IgnoreTab::cancelChanges() {
    if(this->getChangeEvents().size() > 0) {
        this->initIgnoreList();
        this->getChangeEvents().clear();
    }
}

QTableWidget* IgnoreTab::getTable() {
    return alterDialog->getIgnoreTable();
}

QPushButton* IgnoreTab::getApplyButton() {
    return alterDialog->getApplyButton();
}

QList<QDockWidget*> IgnoreTab::getDockWindows() {
    return alterDialog->getDockWindows();
}
