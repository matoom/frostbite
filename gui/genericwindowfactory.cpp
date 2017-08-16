#include "genericwindowfactory.h"

GenericWindowFactory::GenericWindowFactory(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    settings = GeneralSettings::getInstance();
}

QPalette GenericWindowFactory::palette() {
    QPalette palette = QPalette();

    QColor textBackground = settings->dockWindowBackground();
    palette.setColor(QPalette::Base, textBackground);

    QColor textColor = settings->dockWindowFontColor();
    palette.setColor(QPalette::Text, textColor);

    return palette;
}

QPlainTextEdit* GenericWindowFactory::textBox(QString name) {
    QFont font = settings->getParameter("DockWindow/font",
        QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE)).value<QFont>();

    GenericWindow* genericWindow = new GenericWindow(mainWindow);
    ((QPlainTextEdit*)genericWindow)->setFont(font);
    ((QPlainTextEdit*)genericWindow)->setObjectName(name);
    ((QPlainTextEdit*)genericWindow)->setPalette(this->palette());

    return (QPlainTextEdit*)genericWindow;
}

QDockWidget* GenericWindowFactory::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mainWindow);        
    dock->setObjectName(QObject::tr(name));
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(this->textBox(name));    
    return dock;
}

GenericWindowFactory::~GenericWindowFactory() {
    delete settings;
}
