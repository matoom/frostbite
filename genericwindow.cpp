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

QTextEdit* GenericWindow::textBox(QDockWidget *dock) {
    QTextEdit *textEdit = new QTextEdit(dock);
    textEdit->setPlainText("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.");
    textEdit->setPalette(this->palette());
    textEdit->setFontWeight(QFont::Normal);
    textEdit->setFont(QFont("Fixedsys", 12));
    textEdit->setReadOnly(true);

    return textEdit;
}

QDockWidget* GenericWindow::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mw);
    dock->setObjectName(name);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);

    dock->setWidget(this->textBox(dock));

    return dock;
}
