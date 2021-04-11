#include "linkstab.h"

#include "text/alter/alterdialog.h"
#include "text/alter/altersettingsentry.h"
#include "text/alter/linksettings.h"

#include "globaldefines.h"
#include "defaultvalues.h"

#include <QHeaderView>

LinksTab::LinksTab(QObject *parent) : QObject(parent), AbstractTableTab() {
    alterDialog = (AlterDialog*)parent;

    linksTable = alterDialog->getLinksTable();
    addButton = alterDialog->getLinksAddButton();
    removeButton = alterDialog->getLinksRemoveButton();

    settings = LinkSettings::getInstance();

    QStringList labels;
    labels << "Regular expression" << "Link";
    linksTable->setColumnCount(labels.count());
    linksTable->setHorizontalHeaderLabels(labels);

    linksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    linksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    linksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    linksTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addNewTableRow()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeTableRow()));
    connect(linksTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayMenu(QPoint)));
    connect(linksTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateEntry(QTableWidgetItem*)));

    this->initLinksList();
}

void LinksTab::updateSettings() {
    settings = LinkSettings::getInstance();
    this->initLinksList();
}

void LinksTab::print(QString text) {
    qDebug() << text;
}

void LinksTab::addNewTableRow() {
   AbstractTableTab::addNewTableRow(QStringList() << WINDOW_TITLE_MAIN);
}

void LinksTab::removeTableRow() {
    AbstractTableTab::removeTableRow();
}

void LinksTab::updateEntry(QTableWidgetItem* item) {
   AbstractTableTab::updateEntry(item);
}

void LinksTab::displayMenu(QPoint pos) {
    AbstractTableTab::displayMenu(pos);
}

void LinksTab::initLinksList() {
    this->setSettingEntries(settings->getLinks());

    linksTable->blockSignals(true);

    linksTable->clearContents();
    linksTable->setRowCount(this->getSettingEntries().size());

    for(int i = 0; i < this->getSettingEntries().size(); i++) {
        AlterSettingsEntry entry = this->getSettingEntries().at(i);
        this->populateTableRow(i, entry);
    }
    linksTable->blockSignals(false);
}

void LinksTab::populateTableRow(int row, AlterSettingsEntry entry) {
    QTableWidgetItem* patternItem = new QTableWidgetItem(entry.pattern);
    patternItem->setData(Qt::UserRole, "pattern");

    if(QRegularExpression(entry.pattern).isValid()) {
        patternItem->setBackgroundColor(QColor(Qt::transparent));
    } else {
        patternItem->setBackgroundColor(QColor(REGEX_ERROR_COLOR_HEX));
    }

    linksTable->setItem(row, 0, patternItem);

    QTableWidgetItem* linkItem = new QTableWidgetItem(entry.value);
    linkItem->setData(Qt::UserRole, "value");
    linksTable->setItem(row, 1, linkItem);
}

void LinksTab::saveChanges() {
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

void LinksTab::cancelChanges() {
    if(this->getChangeEvents().size() > 0) {
        this->initLinksList();
        this->getChangeEvents().clear();
    }
}

QTableWidget* LinksTab::getTable() {
    return alterDialog->getLinksTable();
}

QPushButton* LinksTab::getApplyButton() {
    return alterDialog->getApplyButton();
}

QList<QDockWidget*> LinksTab::getDockWindows() {
    return alterDialog->getDockWindows();
}

LinksTab::~LinksTab() {
}
