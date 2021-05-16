#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <QObject>
#include <QAction>
#include <QDebug>
#include <QMenu>

class ConnectWizard;
class MainWindow;
class HighlightDialog;
class AppearanceDialog;
class AlterDialog;
class AboutDialog;
class ScriptEditDialog;
class ProfileAddDialog;
class ScriptSettingsDialog;
class VolumeControlDialog;
class DictionaryDialog;
class ClientSettings;
class WindowFacade;
class MacroDialog;

#define USER_GUIDE_URL "https://matoom.github.com/frostbite"

class MenuHandler : public QObject {
    Q_OBJECT

public:
    MenuHandler(QObject *parent = 0);
    ~MenuHandler();

    void openConnectDialog();
    void openAppearanceDialog();

private:
    MainWindow* mainWindow;
    ClientSettings* clientSettings;

    ConnectWizard* connectWizard;
    HighlightDialog* highlightDialog;
    MacroDialog* macroDialog;
    AlterDialog* alterDialog;
    AppearanceDialog* appearanceDialog;
    ScriptSettingsDialog* scriptSettingsDialog;
    AboutDialog* aboutDialog;
    ScriptEditDialog* scriptEditDialog;
    ProfileAddDialog* profileAddDialog;
    WindowFacade* windowFacade;
    VolumeControlDialog* volumeControlDialog;
    DictionaryDialog* dictionaryDialog;

    QMenu* profilesMenu;
    QAction* action;

    bool menuReady;

    void loadLoggingMenu();
    void loadToolbarMenu();
    void loadWindowMenu();
    void loadCompassMenu();

signals:
    void compassLocked(bool);
    void compassVisible(bool);
    void compassAnchored(bool);
    void resetCompass();

public slots:
    void menuTriggered(QAction*);
    void menuHovered(QAction*);
    void setMenuReady();
    void profileTriggered(QAction* action);
    void loadProfilesMenu();    
    void reloadSettings();
    void lockWindows(QAction* action);
};

#endif // MENUHANDLER_H
