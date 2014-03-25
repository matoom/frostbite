#include <guiapplication.h>
#include "mainwindow.h"

#include "log4qt/logger.h"
#include <log4qt/propertyconfigurator.h>

bool MainWindow::DEBUG = true;

int main(int argc, char *argv[]) {
    /* Prohibit running more than one copy of appliction to
       prevent any conflicts with shared library shared memory.
    */

    QSharedMemory shared(QDir::currentPath());
    if(!shared.create(512, QSharedMemory::ReadWrite)) {
        exit(0);
    }

    Log4Qt::PropertyConfigurator::configure(QDir::currentPath() + "/log.ini");

    try {
        GuiApplication a(argc, argv);

        MainWindow w;
        w.show();

        if(!MainWindow::DEBUG) {
            w.openConnectDialog();
        }

        return a.exec();
    } catch (std::exception & e) {
        Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info(e.what());
        qFatal(e.what());
    }
    return 0;
}
