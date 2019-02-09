#include "contextmenu.h"

ContextMenu::ContextMenu(QWidget *parent) : QMenu(parent) {
}

void ContextMenu::mousePressEvent(QMouseEvent* event) {
    QMenu::mousePressEvent(event);
}

void ContextMenu::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        QMenu::mouseReleaseEvent(event);
    }
}
