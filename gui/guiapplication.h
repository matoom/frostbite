#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

#include <QMessageBox>
#include <singleapp/QtSingleApplication>

#include <typeinfo>

#include "log4qt/logger.h"

class GuiApplication : public QtSingleApplication {
    Q_OBJECT
public:
    GuiApplication(int &argc, char **argv);
    virtual ~GuiApplication() { }

    bool notify(QObject* receiver, QEvent* event);

signals:

public slots:

};

#endif // GUIAPPLICATION_H
