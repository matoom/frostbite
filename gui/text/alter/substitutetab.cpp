#include "substitutetab.h"

SubstituteTab::SubstituteTab(QObject *parent) : QObject(parent), AbstractTableTab() {
    alterDialog = (AlterDialog*)parent;

    substitutionTable = alterDialog->getSubstitutionTable();
    addButton = alterDialog->getSubstitutionAddButton();
    removeButton = alterDialog->getSubstitutionRemoveButton();

    settings = SubstitutionSettings::getInstance();

    QStringList labels;
    labels << "Pattern" << "Substitute";
    substitutionTable->setColumnCount(labels.count());
    substitutionTable->setHorizontalHeaderLabels(labels);

    substitutionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    substitutionTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    substitutionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    substitutionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    substitutionTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addNewTableRow()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeTableRow()));
    connect(substitutionTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayMenu(QPoint)));
    connect(substitutionTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateEntry(QTableWidgetItem*)));

    this->initSubstitutionList();
}

void SubstituteTab::print(QString text) {
    qDebug() << text;
}

void SubstituteTab::updateSettings() {
    settings = SubstitutionSettings::getInstance();
    this->initSubstitutionList();
}

void SubstituteTab::addNewTableRow() {
   AbstractTableTab::addNewTableRow();
}

void SubstituteTab::removeTableRow() {
    AbstractTableTab::removeTableRow();
}

void SubstituteTab::updateEntry(QTableWidgetItem* item) {
   AbstractTableTab::updateEntry(item);
}

void SubstituteTab::displayMenu(QPoint pos) {
    AbstractTableTab::displayMenu(pos);
}

void SubstituteTab::initSubstitutionList() {
    this->setSettingEntries(settings->getSubstitutions());

    substitutionTable->blockSignals(true);

    substitutionTable->clearContents();
    substitutionTable->setRowCount(this->getSettingEntries().size());

    for(int i = 0; i < this->getSettingEntries().size(); i++) {
        AlterSettingsEntry entry = this->getSettingEntries().at(i);
        this->populateTableRow(i, entry);
    }
    substitutionTable->blockSignals(false);
}

void SubstituteTab::populateTableRow(int row, AlterSettingsEntry entry) {
    QTableWidgetItem* patternItem = new QTableWidgetItem(entry.pattern);
    patternItem->setData(Qt::UserRole, "pattern");

    if(QRegularExpression(entry.pattern).isValid()) {
        patternItem->setBackgroundColor(QColor(Qt::transparent));
    } else {
        patternItem->setBackgroundColor(QColor(REGEX_ERROR_COLOR_HEX));
    }

    substitutionTable->setItem(row, 0, patternItem);

    QTableWidgetItem* substituteItem = new QTableWidgetItem(entry.substitute);
    substituteItem->setData(Qt::UserRole, "substitute");
    substitutionTable->setItem(row, 1, substituteItem);
}

void SubstituteTab::saveChanges() {
    // rewrite all settings if any remove events
    if(hasAny(this->getChangeEvents(), TableChangeEvent::Remove)) {
        settings->setSettings(this->getSettingEntries());
    } else {
        for(int id : this->getChangeEvents().keys()) {
            AlterSettingsEntry entry = this->getSettingEntries().at(id);
            QList<TableChangeEvent> changeEvents = this->getChangeEvents().value(id);
            if(changeEvents.contains(TableChangeEvent::Add)) {
                settings->addParameter(entry);
            } else if(changeEvents.contains(TableChangeEvent::Update)) {
                settings->setParameter(entry);
            }
        }
    }
    // does not work
    this->getChangeEvents().clear();
}

void SubstituteTab::cancelChanges() {
    if(this->getChangeEvents().size() > 0) {
        this->initSubstitutionList();
        this->getChangeEvents().clear();
    }
}

QTableWidget* SubstituteTab::getTable() {
    return alterDialog->getSubstitutionTable();
}

QPushButton* SubstituteTab::getApplyButton() {
    return alterDialog->getApplyButton();
}

QList<QDockWidget*> SubstituteTab::getDockWindows() {
    return alterDialog->getDockWindows();
}

SubstituteTab::~SubstituteTab() {
}
