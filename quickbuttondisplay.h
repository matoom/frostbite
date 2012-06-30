#ifndef QUICKBUTTONDISPLAY_H
#define QUICKBUTTONDISPLAY_H

#include <QObject>
#include <QtGui>

#include <toolbardefines.h>
#include <toolbarmanager.h>

class ToolbarManager;

class QuickButtonDisplay : public QObject {
    Q_OBJECT

public:
    explicit QuickButtonDisplay(QObject *parent = 0);

    QWidget *create();

private:
    QToolButton *actionButton(const char*, const char*, const char*);
    ToolbarManager *toolbarManager;

signals:

public slots:

};

#endif // QUICKBUTTONDISPLAY_H
