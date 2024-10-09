#include "windowwriterthread.h"
#include "text/highlight/highlighter.h"
#include "text/alter/alter.h"
#include "mainwindow.h"
#include "windowinterface.h"
#include "globaldefines.h"
#include "genericwindow.h"

WindowWriterThread::WindowWriterThread(QObject *parent, WindowInterface* window) {
    mainWindow = (MainWindow*)parent;
    this->textEdit = dynamic_cast<QPlainTextEdit*>(window);
    this->window = window;

    highlighter = new Highlighter(parent);
    alter = new Alter();
    connect(this, &WindowWriterThread::finished, alter, &QObject::deleteLater);

    GenericWindow* genericWindow = dynamic_cast<GenericWindow*>(window);
    if(genericWindow != NULL) {
        connect(this, SIGNAL(writeStream(const QString&)), genericWindow, SLOT(appendHtmlStream(const QString&)));
    }

    connect(this, SIGNAL(writeText(const QString&)), this->textEdit, SLOT(appendHtml(const QString&)));
    connect(this, SIGNAL(clearText()), this->textEdit, SLOT(clear()));
}

void WindowWriterThread::updateSettings() {   
    highlighter->reloadSettings();
    alter->reloadSettings();
}

void WindowWriterThread::addText(QString text) {
    Parent::addData(text);
}

QString WindowWriterThread::process(QString text, QString win) {
    return highlighter->highlight(alter->addLink(alter->substitute(text, win), win));
}

void WindowWriterThread::onProcess(const QString& data) {
    if(alter->ignore(data, window->getObjectName())) return;
    if(window->stream()) {
        if(data.startsWith("{clear}")) {
            emit clearText();
        } else {
            emit writeStream(this->process(data, window->getObjectName()));
        }
    } else if(window->append()) {
        setText(this->process(data, window->getObjectName()));
    } else {
        QString text = "";
        QList<QString> lines = data.split('\n');
        int size = lines.size() - 1;
        for(int i = 0; i < size; ++i) {
            text += this->process(lines.at(i), window->getObjectName());

            if(i < size - 1) {
                text += "\n";
            }
        }
        emit clearText();
        setText(text);
    }
}

void WindowWriterThread::setText(QString text) {
    emit writeText("<span class=\"body\">"
                   + (text.isEmpty() ? "&nbsp;" : text) +
                   "</span>");
}
