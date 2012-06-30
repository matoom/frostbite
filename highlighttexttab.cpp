#include "highlighttexttab.h"

HighlightTextTab::HighlightTextTab(QObject *parent) : QObject(parent) {
    settings = HighlightSettings::Instance();

    highlightDialog = (HighlightDialog*)parent;
    add = highlightDialog->getTextAddButton();
    listWidget = highlightDialog->getTextList();

    highlightAddDialog = new HighlightAddDialog(this, highlightDialog);
    addGroupSelect = highlightAddDialog->getGroupSelect();

    groupNames << "All" << "General" << "Names" << "Monsters" << "Travel" << "Combat" << "Various";

    initAddDialog();

    listWidget->setStyleSheet("QListWidget {background-color: black;}");

    connect(add, SIGNAL(clicked()), this, SLOT(addHighlightDialog()));
}

/*
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
}*/

void HighlightTextTab::addHighlight() {
    //qDebug() << "ADD::";
    //HighlightSettingsEntry he = HighlightSettingsEntry("", "kurk", QColor(0, 0, 0), QHash<QString, QString>(), QHash<QString, QString>(), 1);
    //qDebug() << he.key;

    //settings->setParameter("1", HighlightSettingsEntry("1"));
}

void HighlightTextTab::initAddDialog() {
    for(int i = 1; i < groupNames.size(); i++) {
        addGroupSelect->addItem(groupNames.value(i), QVariant(i));
    }
}

void HighlightTextTab::addHighlightDialog() {
    highlightAddDialog->show();
}

