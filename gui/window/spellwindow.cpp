#include "spellwindow.h"

SpellWindow::SpellWindow(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = (WindowFacade*)mainWindow->getWindowFacade();

    dock = GenericWindowFactory(parent).createWindow(DOCK_TITLE_SPELL);
    window = (GenericWindow*)dock->widget();
    window->setAppend(false);

    writer = new WindowWriterThread(mainWindow, window);

    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, dock);

    connect(windowFacade, SIGNAL(updateWindowSettings()), writer, SLOT(updateSettings()));
}

QDockWidget* SpellWindow::getDockWidget() {
    return dock;
}

void SpellWindow::write(QString text) {
    text = text.remove(QRegularExpression("\\s+(roisaen|roisan)"));
    text = text.replace(QRegularExpression("Indefinite"), "Cyclic");
    writer->addText(text + "\n");
    if(!writer->isRunning()) writer->start();
}

SpellWindow::~SpellWindow() {
    delete writer;
}
