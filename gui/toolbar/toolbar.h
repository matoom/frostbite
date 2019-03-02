#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QObject>

#include <mainwindow.h>
#include <toolbar/vitalsindicator.h>
#include <toolbar/statusindicator.h>
#include <toolbar/wieldindicator.h>
#include <toolbar/spellindicator.h>
#include <toolbar/quickbuttondisplay.h>
#include <toolbar/quickbuttoneditdialog.h>
#include <toolbar/fullscreenbutton.h>
#include <toolbar/activespellindicator.h>
#include <toolbar/mutebutton.h>
#include <gamedatacontainer.h>

class MainWindow;
class QuickButtonDisplay;
class QuickButtonEditDialog;
class GameDataContainer;
class FullscreenButton;
class MuteButton;

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
    MuteButton* muteButton;

    ClientSettings* clientSettings;

    QAction* wieldLeftAction;
    QAction* wieldRightAction;
    QAction* spellAction;
    QAction* activeSpellAction;
    QAction* statusAction;
    QAction* buttonsAction;
    QAction* vitalsAction;
    QAction* muteButtonAction;

    void addFullScreenButton();
    void addMuteButton();

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
    void setMuteVisible(bool visible);

    void reloadSettings();
};

#endif // TOOLBAR_H
