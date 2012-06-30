#ifndef HIGHLIGHTTEXTTAB_H
#define HIGHLIGHTTEXTTAB_H

#include <QObject>

#include <highlightdialog.h>
#include <highlightadddialog.h>
#include <highlightsettings.h>
#include <highlightsettingsentry.h>

class HighlightDialog;
class HighlightAddDialog;
class HighlightSettings;

namespace HighLight {
    enum Flags {SingleWord = 1,
                EntireRow = 2,
                PartialMatches = 4,
                StartingWith = 8};
}

namespace Group {
    enum List {All, Names, Monsters, Travel, Combat, Various};
}

class HighlightTextTab : public QObject {
    Q_OBJECT
public:
    explicit HighlightTextTab(QObject *parent = 0);

private:
    HighlightDialog *highlightDialog;
    HighlightAddDialog *highlightAddDialog;
    HighlightSettings *settings;

    QHash<QString, QHash<QString, QVariant> > highlightList;

    QPushButton *add;
    QListWidget *listWidget;
    QDialog *addDialog;
    QComboBox *addGroupSelect;
    QList<QString> groupNames;

    void initAddDialog();
    void addHighlight();

signals:

private slots:
    void addHighlightDialog();

public slots:

};

#endif // HIGHLIGHTTEXTTAB_H
