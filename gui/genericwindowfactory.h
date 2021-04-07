#ifndef WINDOWFACTORY_H
#define WINDOWFACTORY_H

#include <QObject>
#include <QDockWidget>
#include <QTextEdit>
#include <QPlainTextEdit>

class MainWindow;
class GeneralSettings;

class GenericWindowFactory: public QObject {
    Q_OBJECT

public:
    GenericWindowFactory(QObject *parent = 0);
    ~GenericWindowFactory();

    QDockWidget* createWindow(const char*);

private:
    QPalette palette();
    QPlainTextEdit* textBox(QString);

    MainWindow* mainWindow;
    GeneralSettings* settings;

private slots:
};

#endif // WINDOWFACTORY_H
