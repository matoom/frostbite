#include "guiapplication.h"

GuiApplication::GuiApplication(int &argc, char **argv) : QtSingleApplication(argc, argv) {
}

bool GuiApplication::notify(QObject* receiver, QEvent* event) {
    try {
        return QApplication::notify(receiver, event);
    } catch (std::exception &e) {
        QString errorMsg = QString("Error %1 sending event %2 to object %3 (%4)")
                .arg(e.what(), typeid(*event).name(), qPrintable(receiver->objectName()), typeid(*receiver).name());

        Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info(errorMsg);
        qFatal(errorMsg.toLocal8Bit().data());
    } catch (...) {
        QString errorMsg = QString("Error <unknown> sending event %1 to object %2 (%3)")
                .arg(typeid(*event).name(), qPrintable(receiver->objectName()), typeid(*receiver).name());

        Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info(errorMsg);
        qFatal(errorMsg.toLocal8Bit().data());
    }
    return false;
}
