#include <guiapplication.h>
#include "mainwindow.h"

#include "log4qt/logger.h"
#include <log4qt/propertyconfigurator.h>

bool MainWindow::DEBUG = false;

int main(int argc, char *argv[]) {
    try {
        GuiApplication a(argc, argv);

        /* Prohibit running more than one copy of appliction to
           prevent any conflicts with shared library shared memory. */
        if(a.isRunning()) {
            Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("Application already running.");
            exit(0);
        }

        Log4Qt::PropertyConfigurator::configure(QApplication::applicationDirPath()  + "/log.ini");

        QApplication::addLibraryPath(QApplication::applicationDirPath());

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
