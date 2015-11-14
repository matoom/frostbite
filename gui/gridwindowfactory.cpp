#include "gridwindowfactory.h"

GridWindowFactory::GridWindowFactory(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    settings = new GeneralSettings();
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

    GridWindow* gridWindow = new GridWindow(mainWindow);
    gridWindow->setFont(font);
    gridWindow->setObjectName(name);
    gridWindow->setPalette(this->palette());
    gridWindow->viewport()->setPalette(this->palette());
    gridWindow->horizontalHeader()->setStretchLastSection(true);
    gridWindow->horizontalHeader()->setVisible(false);
    gridWindow->verticalHeader()->setVisible(false);
    gridWindow->setShowGrid(false);
    gridWindow->setColumnCount(1);

    QHeaderView *verticalHeader = gridWindow->verticalHeader();
    verticalHeader->setDefaultSectionSize(verticalHeader->fontMetrics().height() + 5);

    return (QTableWidget*)gridWindow;
}

QDockWidget* GridWindowFactory::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mainWindow);
    dock->setObjectName(QObject::tr(name) + "Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(this->tableBox(name));

    return dock;
}

GridWindowFactory::~GridWindowFactory() {
    delete settings;
}
