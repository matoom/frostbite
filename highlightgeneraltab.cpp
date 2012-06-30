#include "highlightgeneraltab.h"

HighlightGeneralTab::HighlightGeneralTab(QObject *parent) : QObject(parent) {
    highlightDialog = (HighlightDialog*)parent;
    settings = ClientSettings::Instance();

    listWidget = highlightDialog->getGeneralList();
    alertGroup = highlightDialog->getGeneralAlertGroup();
    fileSelect = highlightDialog->getGeneralFileSelect();
    applyButton = highlightDialog->getApplyButton();

    this->loadSettings();
    this->prepareList();
    this->readSoundFiles();
    this->initContextMenu();

    listWidget->setStyleSheet("QListWidget {background-color: black;}");

    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(itemSelected(QListWidgetItem*, QListWidgetItem*)));
    connect(listWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(listWidgetMenuRequested(const QPoint &)));
    connect(alertGroup, SIGNAL(clicked(bool)), this, SLOT(alertClicked(bool)));
    connect(fileSelect, SIGNAL(activated(const QString&)), this,
            SLOT(fileSelected(const QString&)));
}

void HighlightGeneralTab::initContextMenu() {
    menu = new QMenu(listWidget);
    colorAct = new QAction(QIcon(":/images/open.png"), tr("&Change Color..."), listWidget);
    menu->addAction(colorAct);
    connect(colorAct, SIGNAL(triggered()), this, SLOT(colorDialog()));

    editAct = new QAction(QIcon(":/images/open.png"), tr("&Edit..."), listWidget);
    menu->addAction(editAct);
    editAct->setEnabled(false);
}

void HighlightGeneralTab::colorDialog() {
    QColor color = QColorDialog::getColor(listWidget->currentItem()->textColor(), listWidget, tr("Title"));

    if (color.isValid()) {
        QString currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toString();
        listWidget->currentItem()->setTextColor(color);
        updateSelectedItemColor(listWidget->currentItem());
        highlightList[currentItemKey].insert("color", color);

        registerChange(currentItemKey);
    }
}

void HighlightGeneralTab::listWidgetMenuRequested(const QPoint &point) {
    QPoint globalPos = listWidget->mapToGlobal(point);
    menu->exec(globalPos);
}

void HighlightGeneralTab::readSoundFiles() {
    QStringList filter;
    filter << "*.wav";

    QDir myDir(QDir::currentPath() + "/sound");
    QStringList list = myDir.entryList(filter, QDir::Files, QDir::Name);
    fileSelect->addItems(list);
}

void HighlightGeneralTab::prepareList() {
    QHash<QString, QHash<QString, QVariant> >::const_iterator i = highlightList.constBegin();
    while (i != highlightList.constEnd()) {
        QListWidgetItem *newItem = new QListWidgetItem(tr(i.value().value("name").toByteArray().data()), listWidget);
        newItem->setData(Qt::UserRole, i.key());
        newItem->setTextColor(i.value().value("color").value<QColor>());
        newItem->setFont(QFont("Fixedsys", 12));
        ++i;
    }
}

QHash<QString, QVariant> HighlightGeneralTab::readSettings(QString id, QString name, QColor color) {
    QHash<QString, QVariant> itemSettings;
    itemSettings = settings->getParameter("GeneralHighlights/" + id, NULL).toHash();

    QHash<QString, QVariant> item;
    item.insert("name", name);
    item.insert("color", itemSettings.value("color", color));
    item.insert("alert", itemSettings.value("alert", NULL));

    if(itemSettings.isEmpty()) {
        settings->setParameter("GeneralHighlights/" + id, item);
    }

    return item;
}

void HighlightGeneralTab::loadSettings() {
    highlightList.insert("RoomName", readSettings("RoomName", "[Room titles]", QColor(255, 255, 255, 255)));
    highlightList.insert("GameMessage", readSettings("GameMessage", "Game Messages", QColor(255, 255, 0, 255)));
    highlightList.insert("Say", readSettings("Say", "Someone says ..", QColor(0, 255, 0, 255)));
    highlightList.insert("AlsoSee", readSettings("AlsoSee", "Also see..", QColor(0, 255, 255, 255)));
    highlightList.insert("AlsoHere", readSettings("AlsoHere", "Also here..", QColor(255, 255, 0, 255)));
}

void HighlightGeneralTab::registerChange(QString currentItemKey) {
    if(!generalChangeList.contains(currentItemKey)) {
        generalChangeList.append(currentItemKey);

        if(!applyButton->isEnabled()) {
            applyButton->setEnabled(true);
        }
    }
}

void HighlightGeneralTab::updateAlertSettings(QString key, QVariant value) {
    QString currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toString();
    QHash<QString, QVariant> itemSettings = highlightList.value(currentItemKey);

    QHash<QString, QVariant> alertSettings = itemSettings.value("alert").toHash();
    alertSettings.insert(key, value);

    highlightList[currentItemKey].insert("alert", alertSettings);

    registerChange(currentItemKey);
}

void HighlightGeneralTab::alertClicked(bool on) {
    updateAlertSettings("enabled", on);
}

void HighlightGeneralTab::fileSelected(const QString& text) {
    updateAlertSettings("file", text);
}

void HighlightGeneralTab::itemSelected(QListWidgetItem *current, QListWidgetItem *previous) {
    if(previous) {
        previous->setIcon(QIcon());
    }
    current->setIcon(QIcon(":/nav/images/active/green/e.png"));

    /* change highlight color to item color */
    updateSelectedItemColor(current);

    /* enable/disable controls for list item */
    updateControls(current->data(Qt::UserRole).toString());
}

void HighlightGeneralTab::updateSelectedItemColor(QListWidgetItem *current) {
    /* change highlight color to item color */
    QPalette palette = listWidget->palette();
    palette.setColor(QPalette::HighlightedText, current->textColor());
    palette.setColor(QPalette::Highlight, Qt::transparent);
    listWidget->setPalette(palette);
}

void HighlightGeneralTab::updateListColor(QString key) {
    for (int i = 0; i < listWidget->count(); i++) {
        if(listWidget->item(i)->data(Qt::UserRole) == key) {
            listWidget->item(i)->setTextColor(highlightList[key].value("color").value<QColor>());
            updateSelectedItemColor(listWidget->item(i));
        }
    }
}

void HighlightGeneralTab::updateControls(QString key) {
    QHash<QString, QVariant> itemSettings = highlightList.value(key);
    QHash<QString, QVariant> alertSettings = itemSettings.value("alert").toHash();

    if(!alertSettings.isEmpty()) {
        alertGroup->setChecked(alertSettings.value("enabled").toBool());

        int index = fileSelect->findText(alertSettings.value("file").toString());
        if(index != -1) {
            fileSelect->setCurrentIndex(index);
        } else {
            fileSelect->setCurrentIndex(0);
        }
    } else {
        alertGroup->setChecked(false);
        fileSelect->setCurrentIndex(0);
    }
}

void HighlightGeneralTab::saveChanges() {
    foreach(QString s, generalChangeList) {
        settings->setParameter("GeneralHighlights/" + s, highlightList.value(s));
    }

    generalChangeList.clear();
}

void HighlightGeneralTab::cancelChanges() {
    foreach(QString s, generalChangeList) {
        QHash<QString, QVariant> itemSettings = settings->getParameter("GeneralHighlights/" + s, NULL).toHash();

        if(!itemSettings.isEmpty()) {
            highlightList[s].insert("color", itemSettings.value("color"));
            highlightList[s].insert("alert", itemSettings.value("alert").toHash());
        }

        updateControls(s);
        updateListColor(s);
    }        

    generalChangeList.clear();
}

HighlightGeneralTab::~HighlightGeneralTab() {
}
