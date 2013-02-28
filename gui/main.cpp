#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[]) {
    /* Prohibit running more than one copy of appliction to
       prevent any conflicts with "data" library shared memory.
    */

    QSharedMemory shared(QDir::currentPath());
    if(!shared.create(512, QSharedMemory::ReadWrite)) {
        exit(0);
    }

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
