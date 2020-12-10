#include "highlighttexttab.h"

HighlightTextTab::HighlightTextTab(QObject *parent) : QObject(parent) {    
    highlightSettings = HighlightSettings::getInstance();

    highlightList = highlightSettings->getTextHighlights();
    generalSettings = GeneralSettings::getInstance();

    highlightDialog = (HighlightDialog*)parent;
    audioPlayer = new AudioPlayer(highlightDialog->getMainWindow());

    addButton = highlightDialog->getTextAddButton();
    applyButton = highlightDialog->getApplyButton();
    removeButton = highlightDialog->getTextRemoveButton();    
    alertBox = highlightDialog->getTextAlertGroup();
    alertFileSelect = highlightDialog->getTextFileSelect();    
    entireRowCheck = highlightDialog->getTextEntireRow();
    partialMatchCheck = highlightDialog->getTextPartialMatch();
    groupsMatchCheck = highlightDialog->getTextGroupsMatch();
    caseInsensitiveCheck = highlightDialog->getTextCaseInsensitive();
    startingWithCheck = highlightDialog->getTextStartingWith();
    timerBox = highlightDialog->getTextTimerGroup();
    timerActionSelect = highlightDialog->getTextActionSelect();
    timerValueLine = highlightDialog->getTextTimerValue();
    listWidget = highlightDialog->getTextList();
    groupSelect = highlightDialog->getTextHighlightGroup();
    sortBySelect = highlightDialog->getTextHighlightSortBy();
    filterEdit = highlightDialog->getTextHighlightFilter();

    playButton = highlightDialog->getTextPlayButton();

    highlightAddDialog = new HighlightAddDialog(this, highlightDialog);
    addGroupSelect = highlightAddDialog->getGroupSelect();

    highlightEditDialog = new HighlightEditDialog(this, highlightDialog);
    editGroupSelect = highlightEditDialog->getGroupSelect();

    groupNames << "All" << "General" << "Names" << "Critters" << "Travel" << "Combat" << "Exp" << "Other";
    group = groupNames.at(0);

    timerActionNames << "Restart" << "Ignore";

    listWidget->setProperty("sortingMode", QVariant::fromValue(SortBy::id));
    listWidget->setSortingEnabled(true);

    this->initSortBy();
    this->initGroupSelect();
    this->initTimerActionSelect();
    this->loadHighlightList();
    this->initContextMenu();
    this->setBackground();

    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(itemSelected(QListWidgetItem*, QListWidgetItem*)));

    connect(addButton, SIGNAL(clicked()), this, SLOT(showAddDialog()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeHighlightItem()));

    connect(timerBox, SIGNAL(clicked(bool)), this, SLOT(timerSelected(bool)));
    connect(timerValueLine, SIGNAL(editingFinished()),
            this, SLOT(timerValueChanged()));
    connect(timerActionSelect, SIGNAL(activated(const QString&)),
            this, SLOT(timerActionSelected(const QString&)));

    connect(alertBox, SIGNAL(clicked(bool)), this, SLOT(alertSelected(bool)));
    connect(alertFileSelect, SIGNAL(activated(const QString&)), this, SLOT(alertFileSelected(const QString&)));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playSound()));

    connect(entireRowCheck, SIGNAL(clicked(bool)), this, SLOT(entireRowSelected(bool)));
    connect(partialMatchCheck, SIGNAL(clicked(bool)), this, SLOT(partialMatchSelected(bool)));
    connect(groupsMatchCheck, SIGNAL(clicked(bool)), this, SLOT(matchGroupsSelected(bool)));
    connect(caseInsensitiveCheck, SIGNAL(clicked(bool)), this, SLOT(caseInsensitiveSelected(bool)));
    connect(startingWithCheck, SIGNAL(clicked(bool)), this, SLOT(startingWithSelected(bool)));

    connect(startingWithCheck, SIGNAL(clicked(bool)), this, SLOT(startingWithSelected(bool)));

    connect(groupSelect, SIGNAL(activated(const QString&)),
            this, SLOT(groupSelected(const QString&)));

    connect(sortBySelect, SIGNAL(activated(int)),
            this, SLOT(sortBySelected(int)));

    connect(filterEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(filterList(const QString&)));

    connect(listWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(listWidgetMenuRequested(const QPoint &)));
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(showEditDialog()));
}

void HighlightTextTab::initContextMenu() {
    menu = new ContextMenu(listWidget);
    colorAct = new QAction(QIcon(":/window/images/color.png"), tr("&Change Color..."), listWidget);
    colorAct->setEnabled(false);
    menu->addAction(colorAct);
    connect(colorAct, SIGNAL(triggered()), this, SLOT(colorDialog()));

    menu->addSeparator();

    bgColorAct = new QAction(QIcon(":/window/images/color.png"), tr("&Change Background..."), listWidget);
    bgColorAct->setEnabled(false);
    menu->addAction(bgColorAct);
    connect(bgColorAct, SIGNAL(triggered()), this, SLOT(bgColorDialog()));

    bgClearAct = new QAction(QIcon(), tr("&Clear Background"), listWidget);
    bgClearAct->setEnabled(false);
    menu->addAction(bgClearAct);
    connect(bgClearAct, SIGNAL(triggered()), this, SLOT(clearBgColor()));

    menu->addSeparator();

    editAct = new QAction(QIcon(":/window/images/edit.png"), tr("&Edit..."), listWidget);
    editAct->setEnabled(false);    
    menu->addAction(editAct);
    connect(editAct, SIGNAL(triggered()), this, SLOT(showEditDialog()));
}

void HighlightTextTab::setBackground() {
    bgColor = generalSettings->gameWindowBackground();
    listWidget->setStyleSheet("QListWidget {background-color:" + bgColor.name() + ";}");
}

void HighlightTextTab::listWidgetMenuRequested(const QPoint &point) {
    QPoint globalPos = listWidget->mapToGlobal(point);
    globalPos.rx()--; globalPos.ry()--;
    menu->exec(globalPos);
}

void HighlightTextTab::colorDialog() {
    QColor color = QColorDialog::getColor(listWidget->currentItem()->textColor(),
        listWidget, tr("Select color"));

    if (color.isValid()) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        listWidget->currentItem()->setTextColor(color);

        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.color = color;
        updateSelectedItemColor(listWidget->currentItem());

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::bgColorDialog() {
    QColor color = QColorDialog::getColor(listWidget->currentItem()->textColor(),
        listWidget, tr("Select background color"));

    if (color.isValid()) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        listWidget->currentItem()->setBackgroundColor(color);

        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.bgColor = color;
        updateSelectedItemColor(listWidget->currentItem());
        highlightList.replace(currentId, currentEntry);

        this->registerChange();
    }
}

void HighlightTextTab::clearBgColor() {
    QColor color = QColor();

    int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
    listWidget->currentItem()->setBackgroundColor(QColor(Qt::transparent));

    HighlightSettingsEntry currentEntry = highlightList.at(currentId);
    currentEntry.bgColor = color;
    updateSelectedItemColor(listWidget->currentItem());

    highlightList.replace(currentId, currentEntry);
    this->registerChange();
}

void HighlightTextTab::enableMenuItems() {
    colorAct->setEnabled(true);
    editAct->setEnabled(true);
    bgColorAct->setEnabled(true);
    bgClearAct->setEnabled(true);
}

void HighlightTextTab::updateIcon(QListWidgetItem *current, QListWidgetItem *previous) {
    if(current) {
        listWidget->setSortingEnabled(false);
        if(previous) {
            previous->setIcon(QIcon(":/window/images/icon_ph.png"));
        }
        current->setIcon(QIcon(":/window/images/arrow_right.png"));
        listWidget->setSortingEnabled(true);
    }
}

void HighlightTextTab::updateSelectedItemColor(QListWidgetItem *current) {
    if(current != NULL) {
        QPalette palette = listWidget->palette();
        palette.setColor(QPalette::HighlightedText, current->textColor());
        if(current->backgroundColor().isValid()) {
            palette.setColor(QPalette::Highlight, current->backgroundColor());
            palette.setColor(QPalette::Background, current->backgroundColor());
        } else {
            palette.setColor(QPalette::Highlight, Qt::transparent);
            palette.setColor(QPalette::Base, Qt::transparent);
        }
        listWidget->setPalette(palette);
    }
}

void HighlightTextTab::updateSettings() {
    highlightSettings = HighlightSettings::getInstance();
    generalSettings = GeneralSettings::getInstance();

    highlightEditDialog->updateSettings();
    highlightAddDialog->updateSettings();

    this->setBackground();
}

void HighlightTextTab::populateHighlights() {
    highlightSettings = HighlightSettings::getInstance();
    highlightList = highlightSettings->getTextHighlights();
}

void HighlightTextTab::loadHighlightList() {
    this->populateHighlights();

    for(int i = 0; i < highlightList.size(); i++) {
        HighlightSettingsEntry entry = highlightList.at(i);

        if(!filterText.isEmpty() && !entry.value.contains(filterText, Qt::CaseInsensitive)) continue;

        if(group == groupNames.at(0)) {
            this->createListItem(entry.id, entry.value, entry.color, entry.bgColor);
        } else {
            if(entry.group == group) {
                this->createListItem(entry.id, entry.value, entry.color, entry.bgColor);
            }
        }
    }
    listWidget->sortItems(Qt::AscendingOrder);
}

void HighlightTextTab::createListItem(int id, QString value, QColor color, QColor bgColor) {
    SortableListWidgetItem *newItem = new SortableListWidgetItem(QIcon(":/window/images/icon_ph.png"), value, listWidget);
    newItem->setData(Qt::UserRole, id);
    newItem->setTextColor(color);
    if(bgColor.isValid()) newItem->setBackgroundColor(bgColor);
    newItem->setFont(QFont(DEFAULT_FONT, 12));
}

void HighlightTextTab::reloadHighlightList() {    
    // reload settings for window highlighters
    highlightDialog->reloadHighlighterSettings();

    // reload highlight dialog items
    int row = listWidget->currentRow();
    listWidget->clear();
    this->loadHighlightList();
    listWidget->setCurrentRow(row);
}

void HighlightTextTab::itemSelected(QListWidgetItem *current, QListWidgetItem *previous) {
    /* enable context menu items once item is selected */
    enableMenuItems();

    /* change highlight selected item icon */
    updateIcon(current, previous);

    /* change highlight color to item color */
    updateSelectedItemColor(current);

    /* enable/disable controls for list item */
    updateControls(current);
}

void HighlightTextTab::registerChange() {
    if(listWidget->currentItem() != NULL) {
        int currentItemKey = listWidget->currentItem()->data(Qt::UserRole).toInt();
        if(!changeList.contains(currentItemKey)) {
            changeList.append(currentItemKey);

            if(!applyButton->isEnabled()) {
                applyButton->setEnabled(true);
            }
        }
    }
}

void HighlightTextTab::sortBySelected(int index) {
    this->sort(sortBySelect->itemData(index).value<SortBy>());
}

void HighlightTextTab::filterList(const QString &text) {
    this->filterText = text;
    this->reloadHighlightList();
}

void HighlightTextTab::groupSelected(const QString& group) {
    this->group = group;
    this->reloadHighlightList();
}

void HighlightTextTab::playSound() {
        audioPlayer->play(alertFileSelect->currentText());
}

void HighlightTextTab::alertSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.alert = value;
        currentEntry.alertValue = alertFileSelect->currentText();

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::alertFileSelected(const QString& fileName) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.alertValue = fileName;

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::timerSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.timer = value;

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::timerActionSelected(const QString& text) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.timerAction = text;

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::timerValueChanged() {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();

        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.timerValue = timerValueLine->text().toInt();

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::entireRowSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.options.setBit(0, value);

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::partialMatchSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.options.setBit(1, value);

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::matchGroupsSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.options.setBit(3, value);

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::caseInsensitiveSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.options.setBit(4, value);

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::startingWithSelected(bool value) {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        currentEntry.options.setBit(2, value);

        highlightList.replace(currentId, currentEntry);
        this->registerChange();
    }
}

void HighlightTextTab::updateControls(QListWidgetItem *currentItem) {
    if(currentItem != NULL) {
        HighlightSettingsEntry currentEntry = highlightList.at(currentItem->data(Qt::UserRole).toInt());

        this->updateAlertControl(currentEntry.alert, currentEntry.alertValue);
        this->updateTimerControl(currentEntry.timer, currentEntry.timerValue, currentEntry.timerAction);
        this->updateOptionsControl(currentEntry.options);

        removeButton->setDisabled(false);
    } else {
        this->clearControls();
    }
}

void HighlightTextTab::clearControls() {
    entireRowCheck->setChecked(false);
    entireRowCheck->setDisabled(true);

    partialMatchCheck->setChecked(false);
    partialMatchCheck->setDisabled(true);

    groupsMatchCheck->setChecked(false);
    groupsMatchCheck->setDisabled(true);

    caseInsensitiveCheck->setChecked(false);
    caseInsensitiveCheck->setDisabled(true);

    startingWithCheck->setChecked(false);
    startingWithCheck->setDisabled(true);

    timerBox->setDisabled(true);
    timerBox->setChecked(false);

    timerActionSelect->setCurrentIndex(0);
    timerValueLine->setText("");

    alertBox->setDisabled(true);
    alertBox->setChecked(false);

    alertFileSelect->setCurrentIndex(0);

    removeButton->setDisabled(true);
}

void HighlightTextTab::updateOptionsControl(QBitArray options) {
    entireRowCheck->setDisabled(false);
    partialMatchCheck->setDisabled(false);
    groupsMatchCheck->setDisabled(false);
    caseInsensitiveCheck->setDisabled(false);
    startingWithCheck->setDisabled(false);

    if(options.isNull()) {
        entireRowCheck->setChecked(false);
        partialMatchCheck->setChecked(false);
        groupsMatchCheck->setChecked(false);
        caseInsensitiveCheck->setCheckable(false);
        startingWithCheck->setChecked(false);
    } else {
        entireRowCheck->setChecked(options.at(0));
        partialMatchCheck->setChecked(options.at(1));
        startingWithCheck->setChecked(options.at(2));
        groupsMatchCheck->setChecked(options.at(3));
        caseInsensitiveCheck->setChecked(options.at(4));
    }
}

void HighlightTextTab::updateAlertControl(bool alert, QString alertValue) {
    alertBox->setDisabled(false);
    if(alert) {
        alertBox->setChecked(true);
    } else {
        alertBox->setChecked(false);
    }

    int index = alertFileSelect->findText(alertValue);
    if(index != -1) {
        alertFileSelect->setCurrentIndex(index);
    } else {
        alertFileSelect->setCurrentIndex(0);
    }
}

void HighlightTextTab::updateTimerControl(bool timer, int timerValue, QString action) {
    timerBox->setDisabled(false);
    if(timer) {
        timerBox->setChecked(true);
    } else {
        timerBox->setChecked(false);
    }

    timerValueLine->setText(QString::number(timerValue));

    int index = timerActionSelect->findText(action);
    if(index != -1) {
        timerActionSelect->setCurrentIndex(index);
    } else {
        timerActionSelect->setCurrentIndex(0);
    }
}

void HighlightTextTab::initSortBy() {
    sortBySelect->addItem("", QVariant::fromValue(SortBy::id));
    sortBySelect->addItem("Alphanumeric", QVariant::fromValue(SortBy::alphanumeric));
    sortBySelect->addItem("Color", QVariant::fromValue(SortBy::color));
}

void HighlightTextTab::initGroupSelect() {
    for(int i = 0; i < groupNames.size(); i++) {
        if(i != 0) {
            addGroupSelect->addItem(groupNames.value(i), QVariant(i));
            editGroupSelect->addItem(groupNames.value(i), QVariant(i));
        }
        groupSelect->addItem(groupNames.value(i), QVariant(i));
    }

    alertFileSelect->addItems(audioPlayer->getAudioList());
}

void HighlightTextTab::initTimerActionSelect() {
    for(int i = 0; i < timerActionNames.size(); i++) {
        timerActionSelect->addItem(timerActionNames.value(i), QVariant(i));
    }
}

void HighlightTextTab::removeHighlightItem() {
    this->saveChanges();

    int id = listWidget->currentItem()->data(Qt::UserRole).toInt();

    highlightList.removeAt(id);

    highlightSettings->setSettings("TextHighlight", highlightList);

    this->reloadHighlightList();
}

void HighlightTextTab::saveChanges() {
    if(!changeList.isEmpty()) {
        foreach(int id, changeList) {
            highlightSettings->setParameter("TextHighlight", highlightList.at(id));
        }
        this->reloadHighlightList();
        changeList.clear();
    }
}

void HighlightTextTab::cancelChanges() {
    if(!changeList.isEmpty()) {
        this->reloadHighlightList();

        changeList.clear();

        this->updateControls(listWidget->currentItem());
    }
}

void HighlightTextTab::showAddDialog() {
    highlightAddDialog->show();
}

void HighlightTextTab::sort(SortBy sortBy) {
    listWidget->setProperty("sortingMode", QVariant::fromValue(sortBy));
    listWidget->sortItems(Qt::AscendingOrder);
}

void HighlightTextTab::showEditDialog() {
    if(listWidget->currentItem() != NULL) {
        int currentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        HighlightSettingsEntry currentEntry = highlightList.at(currentId);
        highlightEditDialog->setEntry(currentEntry);
        highlightEditDialog->show();
    }
}

HighlightTextTab::~HighlightTextTab() {
    delete audioPlayer;
}
