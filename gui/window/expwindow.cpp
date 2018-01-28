#include "expwindow.h"

ExpWindow::ExpWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();
    gameDataContainer = GameDataContainer::Instance();

    dock = GridWindowFactory(parent).createWindow(DOCK_TITLE_EXP);
    window = (GridWindow*)dock->widget();

    writer = new GridWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(windowFacade, SIGNAL(updateWindowSettings()), window, SLOT(updateSettings()));
    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
    connect(writer, SIGNAL(writeGrid(GridItems)), this, SLOT(writeExpWindow(GridItems)));
}

QDockWidget* ExpWindow::getDockWidget() {
    return dock;
}

void ExpWindow::write(QString name, QString text) {
    writer->addItem(name, text);
    if(!writer->isRunning()) writer->start();
}

void ExpWindow::writeExpWindow(GridItems items) {
    QTableWidget* table = ((QTableWidget*)dock->widget());

    int size = items.size();
    table->setRowCount(size);

    window->setWindowTitle(QStringLiteral(DOCK_TITLE_EXP) + " (" + QString::number(size) + ")");

    int i = 0;
    foreach(QString key, items.keys()) {
        QString text = "<span style=\"white-space:pre-wrap;\">";
        if(gameDataContainer->isExpGained(key)) {
            text += "(+)";
        } else {
            text += "&nbsp;&nbsp;&nbsp;";
        }
        text += items.value(key) + "</span>";

        QLabel* item = (QLabel*)table->cellWidget(i, 0);
        if(item == NULL) {
            item = window->gridValueLabel(table);
            table->setCellWidget(i, 0, item);
        }
        item->setText(text);
        item->setObjectName(key);
        window->track(key, item);
        i++;
    }
}

ExpWindow::~ExpWindow() {
    delete writer;
}
