#ifndef SORTABLELISTWIDGETITEM_H
#define SORTABLELISTWIDGETITEM_H

#include <QDebug>
#include <QListWidgetItem>
#include <QCollator>

enum SortBy { id, alphanumeric, color };

Q_DECLARE_METATYPE(SortBy)

class SortableListWidgetItem : public QListWidgetItem {

public:
    explicit SortableListWidgetItem(const QIcon &icon, const QString &text, QListWidget *view = Q_NULLPTR, int type = Type);

    bool operator<(const QListWidgetItem &other) const;

private:
    QCollator collator;
    QListWidget *view;

signals:

public slots:
};

#endif // SORTABLELISTWIDGETITEM_H
