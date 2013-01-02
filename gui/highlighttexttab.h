#ifndef HIGHLIGHTTEXTTAB_H
#define HIGHLIGHTTEXTTAB_H

#include <QObject>

#include <highlightdialog.h>
#include <highlightadddialog.h>
#include <highlighteditdialog.h>
#include <highlightsettings.h>
#include <highlightsettingsentry.h>
#include <audioplayer.h>
#include <generalsettings.h>

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
    QCheckBox *startingWithCheck;
    QComboBox *groupSelect;

    QListWidget *listWidget;
    QDialog *addDialog;
    QComboBox *addGroupSelect;
    QComboBox *editGroupSelect;
    QList<QString> groupNames;
    QString group;
    QList<QString> timerActionNames;

    QList<HighlightSettingsEntry> highlightList;
    QList<int> changeList;

    QAction *colorAct;
    QAction *editAct;
    QMenu *menu;

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
    void createListItem(int, QString, QColor);
    void updateSelectedItemColor(QListWidgetItem*);
    void updateIcon(QListWidgetItem*, QListWidgetItem*);
    void enableMenuItems();
    QList<HighlightSettingsEntry> populateHighlights();

signals:

private slots:
    void showAddDialog();
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
    void startingWithSelected(bool);
    void groupSelected(const QString&);
    void listWidgetMenuRequested(const QPoint&);
    void colorDialog();
    void playSound();

public slots:

};

#endif // HIGHLIGHTTEXTTAB_H
