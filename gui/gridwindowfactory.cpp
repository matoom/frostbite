#include "gridwindowfactory.h"
#include "mainwindow.h"
#include "generalsettings.h"
#include "defaultvalues.h"
#include "gridwindow.h"

GridWindowFactory::GridWindowFactory(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    settings = GeneralSettings::getInstance();
}

QPalette GridWindowFactory::palette() {
    QPalette palette = QPalette();

    QColor textBackground = settings->dockWindowBackground();
    palette.setColor(QPalette::Base, textBackground);

    QColor textColor = settings->dockWindowFontColor();
    palette.setColor(QPalette::Text, textColor);

    return palette;
}

QTableWidget* GridWindowFactory::tableBox(QString name) {
    QFont font = settings->getParameter("DockWindow/font",
        QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE)).value<QFont>();

    GridWindow* gridWindow = new GridWindow(name, mainWindow);
    gridWindow->setFont(font);
    gridWindow->setObjectName(name);
    gridWindow->setPalette(this->palette());
    gridWindow->viewport()->setPalette(this->palette());
    gridWindow->horizontalHeader()->setStretchLastSection(true);
    gridWindow->horizontalHeader()->setVisible(false);
    gridWindow->verticalHeader()->setVisible(false);
    gridWindow->setShowGrid(false);
    gridWindow->setColumnCount(1);

    //gridWindow->setStyleSheet("item { margin: -10px; padding: -10px: border: none; }");

    return (QTableWidget*)gridWindow;
}

QDockWidget* GridWindowFactory::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mainWindow);
    dock->setObjectName(QObject::tr(name));
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(this->tableBox(name));

    return dock;
}

GridWindowFactory::~GridWindowFactory() {
}
