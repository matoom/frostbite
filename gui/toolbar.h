#ifndef TOOLBAR_H
#define TOOLBAR_H

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

class Toolbar : public QObject {
    Q_OBJECT

public:
    Toolbar(QObject *parent = 0);
    ~Toolbar();

    MainWindow* getMainWindow();

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
    QuickButtonDisplay* quickButtonDisplay;
    QuickButtonEditDialog* editDialog;
    WieldIndicator* wieldLeft;
    WieldIndicator* wieldRight;
    SpellIndicator* spell;
    ActiveSpellIndicator* activeSpell;

    ClientSettings* clientSettings;

    QAction* wieldLeftAction;
    QAction* wieldRightAction;
    QAction* spellAction;
    QAction* activeSpellAction;
    QAction* statusAction;
    QAction* buttonsAction;
    QAction* vitalsAction;

    void addFullScreenButton();

public slots:
    void quickButtonAction();

    void updateVitals(QString name, QString value);
    void updateStatus(QString visible, QString icon);
    void updateWieldLeft(QString value);
    void updateWieldRight(QString value);
    void updateSpell(QString);

    void updateActiveSpells(QStringList);
    void clearActiveSpells();

    void setWieldRightVisible(bool visible);
    void setWieldLeftVisible(bool visible);
    void setSpellVisible(bool visible);
    void setActiveSpellsVisible(bool visible);
    void setStatusVisible(bool visible);
    void setButtonsVisible(bool visible);
    void setVitalsVisible(bool visible);

    void reloadSettings();
};

#endif // TOOLBAR_H
