#include <QtGui/QApplication>
#include "mainwindow.h"
#include "log4qt/logger.h"
#include <log4qt/propertyconfigurator.h>

int main(int argc, char *argv[]) {
    /* Prohibit running more than one copy of appliction to
       prevent any conflicts with "data" library shared memory.
    */

    QSharedMemory shared(QDir::currentPath());
    if(!shared.create(512, QSharedMemory::ReadWrite)) {
        exit(0);
    }

    Log4Qt::PropertyConfigurator::configure(QDir::currentPath() + "/log.ini");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.openConnectDialog();

    return a.exec();
}
