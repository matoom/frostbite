#include "expwindow.h"

#include "custom/contextmenu.h"
#include "mainwindow.h"
#include "windowfacade.h"
#include "gridwindow.h"
#include "gridwriterthread.h"
#include "gamedatacontainer.h"
#include "defaultvalues.h"
#include "gridwindowfactory.h"

ExpWindow::ExpWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    gameDataContainer = GameDataContainer::Instance();

    dock = GridWindowFactory(parent).createWindow(DOCK_TITLE_EXP);
    table = ((QTableWidget*)dock->widget());

    window = (GridWindow*)dock->widget();

    showGained = window->getWindowParameter("showGained", true).value<bool>();

    writer = new GridWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    this->addContextMenu();

    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
    connect(writer, SIGNAL(writeGrid(GridItems)), this, SLOT(writeExpWindow(GridItems)));
}

void ExpWindow::addContextMenu() {
    ContextMenu* menu = window->getMenu();
    menu->addSeparator();

    gainedAct = new QAction(tr("&Show Gains\t"), this);
    gainedAct->setCheckable(true);
    gainedAct->setChecked(showGained);
    menu->addAction(gainedAct);
    connect(gainedAct, SIGNAL(triggered()), this, SLOT(changeShowGained()));
}

void ExpWindow::changeShowGained() {
    this->showGained = gainedAct->isChecked();
    window->setWindowParameter("showGained", this->showGained);
    this->refresh();
}

QDockWidget* ExpWindow::getDockWidget() {
    return dock;
}

void ExpWindow::write(QString name, QString text) {
    writer->addItem(name, text);
    if(!writer->isRunning()) writer->start();
}

void ExpWindow::refresh() {
    int rows = table->rowCount();
    for(int i = 0; i < rows; i++) {
        QLabel* label = (QLabel*)table->cellWidget(i, 0);
        QString key = label->objectName();
        QString value = label->property("plainText").value<QString>();
        this->writeRow(key, value, i);
    }
}

void ExpWindow::writeExpWindow(GridItems items) {
    int size = items.size();
    table->setRowCount(size);
    int row = 0;
    foreach(QString key, items.keys()) {
        QString value = items.value(key);
        this->writeRow(key, value, row);
        row++;
    }
}

void ExpWindow::writeRow(QString key, QString value, int row) {
    QString text = "<span style=\"white-space:pre-wrap; \">";
    addGainedIndicator(key, text);
    text += value + "</span>";

    QLabel* item = (QLabel*)table->cellWidget(row, 0);
    if(item == NULL) {
        item = window->gridValueLabel(table);
        table->setCellWidget(row, 0, item);
    }
    item->setProperty("plainText", value);
    item->setText(text);
    item->setObjectName(key);
    window->track(key, item);
}

void ExpWindow::addGainedIndicator(QString key, QString &text) {
    if(showGained) {
        if(gameDataContainer->isExpGained(key)) {
            text += "(+)";
        } else {
            text += "&nbsp;&nbsp;&nbsp;";
        }
    }
}

ExpWindow::~ExpWindow() {
    delete writer;
}
