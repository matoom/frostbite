#ifndef TOOLBARFACTORY_H
#define TOOLBARFACTORY_H

#include <QtGui>

#include <toolbardefines.h>

class ToolbarFactory : public QObject {
    Q_OBJECT

public:
    ToolbarFactory(QObject *parent = 0);

    QWidget* createHoldBar(const char*, const char*);
    QWidget* createStatusBar();
    QWidget* createQuickButtonBar();
    QWidget* createVitalsBar();

private:
    QLabel* holdImageLabel(const char*);
    QLabel* holdTextLabel(const char*);
    QLabel* playerStatusLabel(const char*, const char*, bool);
    QToolButton* quickActionButton(const char*, const char*, const char*);
    QProgressBar* vitalsProgress(const char*, const char*, int);

};

#endif // TOOLBARFACTORY_H
