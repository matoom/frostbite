#include "sortablelistwidgetitem.h"

SortableListWidgetItem::SortableListWidgetItem(const QIcon &icon, const QString &text, QListWidget *view, int type) : QListWidgetItem(icon, text, view, type) {
    this->view = view;

    collator.setNumericMode(true);
}

bool SortableListWidgetItem::operator<(const QListWidgetItem &other) const {
    SortBy sortBy = view->property("sortingMode").value<SortBy>();

    QString value;
    QString otherValue;

    if(sortBy == SortBy::id) {
        value = data(Qt::UserRole).value<QString>();
        otherValue = other.data(Qt::UserRole).value<QString>();
    } else if(sortBy == SortBy::alphanumeric) {
        value = this->text();
        otherValue = other.text();
    } else if(sortBy == SortBy::color) {
        value = this->textColor().name();
        otherValue = other.textColor().name();
    }

    return collator.compare(value, otherValue) < 0;
}
