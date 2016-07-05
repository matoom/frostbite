#include "macrodialog.h"
#include "ui_macrodialog.h"

MacroDialog::MacroDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MacroDialog) {
    ui->setupUi(this);

    macroSettings = MacroSettings::Instance();

    saveTime = false;

    this->loadSettings();
    this->loadSequenceTime();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));

    connect(ui->altTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
            this, SLOT(altCommandChanged(QTableWidgetItem*, QTableWidgetItem*)));
    connect(ui->ctrlTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
            this, SLOT(ctrlCommandChanged(QTableWidgetItem*, QTableWidgetItem*)));
    connect(ui->functionTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
            this, SLOT(functionCommandChanged(QTableWidgetItem*, QTableWidgetItem*)));
    connect(ui->keypadTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
            this, SLOT(keypadCommandChanged(QTableWidgetItem*, QTableWidgetItem*)));
    connect(ui->sequenceTime, SIGNAL(textChanged(const QString&)),
            this, SLOT(sequenceTimeChanged(const QString&)));

    ui->altTable->setFocus();
}

void MacroDialog::updateSettings() {
    macroSettings->init();
}

void MacroDialog::clearMacros() {
    keys.clear();
}

void MacroDialog::loadSettings() {
    this->buildKeys("alt", Qt::AltModifier, 0x41, 0x5a);
    this->loadKeys("alt", ui->altTable);
    this->buildKeys("ctrl", Qt::ControlModifier, 0x41, 0x5a);
    this->loadKeys("ctrl", ui->ctrlTable);
    this->buildKeys("function", Qt::NoModifier, 0x01000030, 0x0100003b);
    this->loadKeys("function", ui->functionTable);

    this->buildKeypadKeys(Qt::NoModifier);
    this->buildKeypadKeys(Qt::ControlModifier);

    this->loadKeys("keypad", ui->keypadTable);
}

void MacroDialog::loadSequenceTime() {
    QString time = macroSettings->getParameter("sequence/time", "1000").toString();
    ui->sequenceTime->setText(time);
}

void MacroDialog::buildKeys(QString tabName, Qt::KeyboardModifiers modifers, int start, int end) {
    QList<QKeyEvent> keyList;
    for(int keyCode = start; keyCode <= end; keyCode++) {        
        // mac include modifier in sequence? QT::ALT
        keyList << QKeyEvent(QEvent::User, keyCode, modifers, QKeySequence(keyCode).toString());
    }
    keys[tabName] << keyList;
}

void MacroDialog::loadKeys(QString tabName, QTableWidget* table) {
    table->setRowCount(keys[tabName].length());
    QTableWidgetItem *keyItem;
    QTableWidgetItem *commandItem;
    for(int i = 0; i < keys[tabName].length(); i++) {
        keyItem = new QTableWidgetItem(keys[tabName].at(i).text());
        keyItem->setFlags(keyItem->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsEnabled);
        table->setItem(i, 0, keyItem);

        commandItem = new QTableWidgetItem(macroSettings->getParameter(tabName + "/" +
            QString::number(keys[tabName].at(i).key() | keys[tabName].at(i).modifiers()), "").toString());
        table->setItem(i, 1, commandItem);
    }
}

void MacroDialog::buildKeypadKeys(Qt::KeyboardModifiers modifers) {
    #ifdef Q_OS_MAC
    this->buildKeypadKeys(modifers, 0x2a, 0x2b);
    this->buildKeypadKeys(modifers, 0x2d, 0x2d);
    this->buildKeypadKeys(modifers, 0x2f, 0x2f);
    this->buildKeypadKeys(modifers, 0x31, 0x39);
    this->buildKeypadKeys(modifers, 0x1000005, 0x1000005);
    #else
    this->buildKeypadKeys(modifers, 0x2a, 0x2b);
    this->buildKeypadKeys(modifers, 0x2d, 0x2d);
    this->buildKeypadKeys(modifers, 0x2f, 0x2f);
    this->buildKeypadKeys(modifers, 0x1000005, 0x1000007);
    this->buildKeypadKeys(modifers, 0x100000B, 0x100000B);
    this->buildKeypadKeys(modifers, 0x1000010, 0x1000017);
    #endif
}

void MacroDialog::buildKeypadKeys(Qt::KeyboardModifiers modifers, int start, int end) {
    QList<QKeyEvent> keyList;
    for(int keyCode = start; keyCode <= end; keyCode++) {
        keyList << QKeyEvent(QEvent::User, keyCode, Qt::KeypadModifier | modifers, QKeySequence(modifers | keyCode).toString());
    }        
    keys["keypad"] << keyList;
}

void MacroDialog::altCommandChanged(QTableWidgetItem* item, QTableWidgetItem*) {
    changeList["alt"].insert(item->row(), item);
    ui->applyButton->setEnabled(true);
}

void MacroDialog::ctrlCommandChanged(QTableWidgetItem* item, QTableWidgetItem*) {
    changeList["ctrl"].insert(item->row(), item);
    ui->applyButton->setEnabled(true);
}

void MacroDialog::functionCommandChanged(QTableWidgetItem* item, QTableWidgetItem*) {
    changeList["function"].insert(item->row(), item);
    ui->applyButton->setEnabled(true);
}

void MacroDialog::keypadCommandChanged(QTableWidgetItem* item, QTableWidgetItem*) {
    changeList["keypad"].insert(item->row(), item);
    ui->applyButton->setEnabled(true);
}

void MacroDialog::sequenceTimeChanged(const QString&) {
    saveTime = true;
    ui->applyButton->setEnabled(true);
}

void MacroDialog::okPressed() {
    ui->applyButton->setEnabled(false);
    this->saveChanges();
    this->saveSequenceTime();
    this->accept();
}

void MacroDialog::cancelPressed() {
    ui->applyButton->setEnabled(false);
    this->cancelChanges();
    this->cancelSequenceTime();
    this->reject();
}

void MacroDialog::applyPressed() {
    ui->applyButton->setEnabled(false);
    this->saveChanges();
    this->saveSequenceTime();
}

void MacroDialog::saveSequenceTime() {
    if(saveTime) {
        macroSettings->setParameter("sequence/time", ui->sequenceTime->text());
        saveTime = false;
    }
}

void MacroDialog::cancelSequenceTime() {
    if(saveTime) {
        this->loadSequenceTime();
        saveTime = false;
    }
}

void MacroDialog::saveChanges() {
    QHashIterator<QString, QHash<int, QTableWidgetItem*> > i(changeList);
    while (i.hasNext()) {
        i.next();
        QHashIterator<int, QTableWidgetItem*> j(i.value());
        while (j.hasNext()) {
            j.next();           
            macroSettings->setParameter(i.key() + "/" +
                QString::number((keys[i.key()].at(j.key()).key() |
                                 keys[i.key()].at(j.key()).modifiers())),j.value()->text());
        }
    }
}

void MacroDialog::cancelChanges() {
    QHashIterator<QString, QHash<int, QTableWidgetItem*> > i(changeList);
    while (i.hasNext()) {
        i.next();
        QHashIterator<int, QTableWidgetItem*> j(i.value());
        while (j.hasNext()) {
            j.next();                                    
            j.value()->setText(macroSettings->getParameter(i.key() + "/" +
                QString::number((keys[i.key()].at(j.key()).key() |
                                 keys[i.key()].at(j.key()).modifiers())), "").toString());
        }
    }
    changeList.clear();
}

MacroDialog::~MacroDialog() {
    delete ui;
}
