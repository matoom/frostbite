#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QObject>

#include <mainwindow.h>
#include <vitalsindicator.h>
#include <statusindicator.h>
#include <wieldindicator.h>
#include <spellindicator.h>
#include <quickbuttondisplay.h>
#include <quickbuttoneditdialog.h>
#include <gamedatacontainer.h>
#include <fullscreenbutton.h>
#include <activespellindicator.h>

class MainWindow;
class QuickButtonDisplay;
class QuickButtonEditDialog;
class GameDataContainer;
class FullscreenButton;

class ToolbarManager : public QObject {
    Q_OBJECT

public:
    ToolbarManager(QObject *parent = 0);
    ~ToolbarManager();

    MainWindow* getMainWindow();

    void updateQuickButtonSettings();

    void loadToolbar();

    int getHealthValue();
    int getConcentrationValue();
    int getFatigueValue();
    int getSpiritValue();

    QHash<QString, bool> getStatus();

private:
    MainWindow* mainWindow;
    VitalsIndicator* vitalsIndicator;
    StatusIndicator* statusIndicator;
    GameDataContainer* gameDataContainer;
    DataConverterService* dataConverterService;
    QuickButtonDisplay* quickButtonDisplay;
    QuickButtonEditDialog* editDialog;
    WieldIndicator* wieldLeft;
    WieldIndicator* wieldRight;
    SpellIndicator* spell;
    ActiveSpellIndicator* activeSpell;

    void addFullScreenButton();

public slots:
    void quickButtonAction();

    void updateVitals(QString name, QString value);
    void updateStatus(QString visible, QString icon);
    void updateWieldLeft(QString value);
    void updateWieldRight(QString value);
    void updateSpell(QString);

    void updateActiveSpells();
    void clearActiveSpells();
};

#endif // TOOLBARMANAGER_H
