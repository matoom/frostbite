#ifndef HIGHLIGHTTEXTTAB_H
#define HIGHLIGHTTEXTTAB_H

#include <QObject>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QComboBox>

#include "text/highlight/highlightsettingsentry.h"
#include "text/highlight/sortablelistwidgetitem.h"

class HighlightDialog;
class HighlightAddDialog;
class HighlightEditDialog;
class HighlightSettings;
class AudioPlayer;
class GeneralSettings;


namespace Group {
    enum List {All, Names, Critters, Travel, Combat, Other};
}

class HighlightTextTab : public QObject {
    Q_OBJECT
public:
    explicit HighlightTextTab(QObject *parent = 0);
    ~HighlightTextTab();

    void loadHighlightList();
    void reloadHighlightList();
    void saveChanges();
    void cancelChanges();    
    void updateSettings();

    QColor bgColor;

private:
    HighlightDialog *highlightDialog;
    HighlightAddDialog *highlightAddDialog;
    HighlightEditDialog *highlightEditDialog;
    HighlightSettings *highlightSettings;
    GeneralSettings* generalSettings;
    AudioPlayer *audioPlayer;

    QPushButton *addButton;
    QPushButton *applyButton;
    QPushButton *removeButton;
    QGroupBox *alertBox;
    QComboBox *alertFileSelect;
    QPushButton *playButton;
    QGroupBox *timerBox;
    QComboBox  *timerActionSelect;
    QLineEdit *timerValueLine;
    QCheckBox *entireRowCheck;
    QCheckBox *partialMatchCheck;
    QCheckBox *groupsMatchCheck;
    QCheckBox *caseInsensitiveCheck;
    QCheckBox *startingWithCheck;
    QComboBox *groupSelect;
    QComboBox *sortBySelect;
    QLineEdit *filterEdit;

    QListWidget *listWidget;
    QDialog *addDialog;
    QComboBox *addGroupSelect;
    QComboBox *editGroupSelect;
    QList<QString> groupNames;
    QString group;
    QString filterText;
    QList<QString> timerActionNames;

    QList<HighlightSettingsEntry> highlightList;
    QList<int> changeList;

    QAction *colorAct;    
    QAction *editAct;
    QAction *bgColorAct;
    QAction *bgClearAct;
    QMenu *menu;

    void initSortBy();
    void initGroupSelect();
    void initTimerActionSelect();
    void initContextMenu();
    void setBackground();
    void updateControls(QListWidgetItem*);
    void clearControls();
    void updateAlertControl(bool, QString);
    void updateTimerControl(bool, int, QString);
    void updateOptionsControl(QBitArray);
    void registerChange();
    void createListItem(int, QString, QColor, QColor);
    void updateSelectedItemColor(QListWidgetItem*);   
    void updateIcon(QListWidgetItem*, QListWidgetItem*);
    void enableMenuItems();
    void populateHighlights();

signals:

private slots:
    void showAddDialog();
    void sort(SortBy);
    void showEditDialog();
    void removeHighlightItem();
    void itemSelected(QListWidgetItem*, QListWidgetItem*);
    void timerValueChanged();
    void alertSelected(bool);
    void alertFileSelected(const QString&);
    void timerSelected(bool);
    void timerActionSelected(const QString&);
    void entireRowSelected(bool);
    void partialMatchSelected(bool);
    void matchGroupsSelected(bool);
    void caseInsensitiveSelected(bool);
    void startingWithSelected(bool);
    void groupSelected(const QString&);
    void listWidgetMenuRequested(const QPoint&);
    void colorDialog();
    void bgColorDialog();
    void clearBgColor();
    void playSound();
    void sortBySelected(int index);
    void filterList(const QString&);

public slots:

};

#endif // HIGHLIGHTTEXTTAB_H
