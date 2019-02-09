#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QDebug>

class ContextMenu : public QMenu {
    Q_OBJECT

public:
    explicit ContextMenu(QWidget *parent = 0);

private:

protected:
   virtual void mousePressEvent(QMouseEvent *event);
   virtual void mouseReleaseEvent(QMouseEvent* event);

signals:

public slots:        
};

#endif // CONTEXTMENU_H
