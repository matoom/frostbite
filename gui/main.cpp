#include <guiapplication.h>
#include "mainwindow.h"

#include "log4qt/logger.h"
#include <log4qt/propertyconfigurator.h>

#include "scriptapiserver.h"

#include "maps/mapreader.h"

bool MainWindow::DEBUG = false;

int main(int argc, char *argv[]) {

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

    QStringList args = QCoreApplication::arguments();
    if(!args.isEmpty() && args.count() > 1) {
        if (QFile(args.at(1)).exists()) {
            QSettings settings(args.at(1), QSettings::IniFormat);
            if(settings.contains("GAMEHOST") && settings.contains("GAMEPORT") && settings.contains("KEY")){
              w.openConnection(settings.value("GAMEHOST").toString(),
                               settings.value("GAMEPORT").toString(),
                               settings.value("KEY").toString());

            }
            return a.exec();
        }
    }

    if(!MainWindow::DEBUG) {
        w.openConnectDialog();
    }
    return a.exec();
}
