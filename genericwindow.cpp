#include "genericwindow.h"

GenericWindow::GenericWindow(QObject *parent) : QObject(parent) {
    mw = (MainWindow*)parent;
    settings = ClientSettings::Instance();
}

QPalette GenericWindow::palette() {    
    QPalette palette = QPalette();

    QColor textBackground = settings->getParameter("DockWindow/background",
        DEFAULT_DOCK_BACKGROUND).value<QColor>();
    palette.setColor(QPalette::Base, textBackground);

    QColor textColor = settings->getParameter("DockWindow/fontColor",
        DEFAULT_DOCK_FONT_COLOR).value<QColor>();
    palette.setColor(QPalette::Text, textColor);

    return palette;
}

QTextEdit* GenericWindow::textBox(QDockWidget *dock, QString name) {
    QFont font = settings->getParameter("DockWindow/font",
        QFont(DEFAULT_DOCK_FONT, DEFAULT_DOCK_FONT_SIZE)).value<QFont>();

    QTextEdit *textEdit = new QTextEdit(dock);
    textEdit->setReadOnly(true);
    textEdit->setObjectName(name + "Text");
    textEdit->setPalette(this->palette());        
    textEdit->setFont(font);
    //textEdit->setFocusPolicy(Qt::NoFocus);

    return textEdit;
}

QDockWidget* GenericWindow::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mw);
    dock->setObjectName(QObject::tr(name) + "Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(this->textBox(dock, name));

    return dock;
}
