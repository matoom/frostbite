#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QObject>
#include <QAction>

class MainWindow;
class QuickButtonDisplay;
class QuickButtonEditDialog;
class GameDataContainer;
class FullscreenButton;
class MuteButton;
class WieldIndicator;
class VitalsIndicator;
class StatusIndicator;
class ActiveSpellIndicator;
class SpellIndicator;
class ClientSettings;
class Highlighter;

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
    Highlighter* highlighter;

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
