#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

#include <QApplication>
#include <QMessageBox>

#include <typeinfo>

#include "log4qt/logger.h"

class GuiApplication : public QApplication {
    Q_OBJECT
public:
    GuiApplication(int &argc, char **argv);
    virtual ~GuiApplication() { }

    bool notify(QObject* receiver, QEvent* event);

signals:

public slots:

};

#endif // GUIAPPLICATION_H
