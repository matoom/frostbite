#include "genericwindowfactory.h"

GenericWindowFactory::GenericWindowFactory(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    settings = ClientSettings::Instance();    
}

QPalette GenericWindowFactory::palette() {
    QPalette palette = QPalette();

    QColor textBackground = settings->getParameter("DockWindow/background",
        DEFAULT_DOCK_BACKGROUND).value<QColor>();
    palette.setColor(QPalette::Base, textBackground);

    QColor textColor = settings->getParameter("DockWindow/fontColor",
        DEFAULT_DOCK_FONT_COLOR).value<QColor>();
    palette.setColor(QPalette::Text, textColor);

    return palette;
}

QPlainTextEdit* GenericWindowFactory::textBox(QString name) {
    QFont font = settings->getParameter("DockWindow/font",
        QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE)).value<QFont>();

    GenericWindow* genericWindow = new GenericWindow(mainWindow);
    ((QPlainTextEdit*)genericWindow)->setFont(font);
    ((QPlainTextEdit*)genericWindow)->setObjectName(name + "Text");
    ((QPlainTextEdit*)genericWindow)->setPalette(this->palette());

    return (QPlainTextEdit*)genericWindow;
}

QDockWidget* GenericWindowFactory::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mainWindow);
    dock->setObjectName(QObject::tr(name) + "Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(this->textBox(name));

    return dock;
}
