#ifndef LINKSTAB_H
#define LINKSTAB_H

#include <QObject>

#include "text/alter/abstracttabletab.h"

class AlterDialog;
class AlterSettingsEntry;
class LinkSettings;

class LinksTab : public QObject, AbstractTableTab {
    Q_OBJECT

public:
    explicit LinksTab(QObject *parent = 0);
    ~LinksTab();

    void updateSettings();
    void saveChanges();
    void cancelChanges();

    void print(QString text);

    QTableWidget* getTable();
    QPushButton* getApplyButton();
    QList<QDockWidget*> getDockWindows();

private:
    AlterDialog* alterDialog;

    QTableWidget* linksTable;
    QPushButton* addButton;
    QPushButton* removeButton;

    LinkSettings* settings;

    void initLinksList();

    void populateTableRow(int row, AlterSettingsEntry entry);

public slots:
    void removeTableRow();
    void addNewTableRow();
    void displayMenu(QPoint);
    void updateEntry(QTableWidgetItem*);
};

#endif // LINKSTAB_H
