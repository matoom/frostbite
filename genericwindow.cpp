#include "genericwindow.h"

GenericWindow::GenericWindow(QObject *parent) : QObject(parent) {
    mw = (MainWindow*)parent;
}

QPalette GenericWindow::palette() {
    QPalette palette = QPalette();
    palette.setColor(QPalette::Base, QColor(0, 0, 0));
    palette.setColor(QPalette::Text, QColor(180, 180, 180));

    return palette;
}

QTextEdit* GenericWindow::textBox(QDockWidget *dock, QString name) {
    QTextEdit *textEdit = new QTextEdit(dock);
    textEdit->setObjectName(name + "Text");
    textEdit->setPalette(this->palette());
    textEdit->setFontWeight(QFont::Normal);
    textEdit->setFont(QFont("Consolas", 11));

    return textEdit;
}

QDockWidget* GenericWindow::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mw);
    dock->setObjectName(QObject::tr(name) + "Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);

    dock->setWidget(this->textBox(dock, name));

    return dock;
}
