#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QPlainTextEdit>
#include <QMouseEvent>
#include <QResizeEvent>

#include <mainwindow.h>
#include <windowfacade.h>
#include <generalsettings.h>
#include <dict/dictionarysettings.h>
#include <defaultvalues.h>
#include <windowinterface.h>
#include <snapshot.h>
#include <custom/contextmenu.h>

class MainWindow;
class WindowFacade;
class Snapshot;
class Compass;
class DictionarySettings;

class GameWindow : public QPlainTextEdit, public WindowInterface {
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    QColor getBgColor();
    QColor getTextColor();
    QTextDocument* getDocument();
    QString getObjectName();
    QPlainTextEdit* getMainWindow();

    void setAppend(bool append);
    bool append();

    void setStream(bool stream);
    bool stream();

private:
    void contextMenuEvent(QContextMenuEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    
    void loadSettings();
    void buildContextMenu();

    void showEvent(QShowEvent*);

    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GeneralSettings* settings;
    DictionarySettings* dictionarySettings;

    Snapshot* snapshot;

    QAction* appearanceAct;
    QAction* lookupDictAct;
    QAction* copyAct;
    QAction* selectAct;
    QAction* clearAct;
    QAction* saveAct;
    ContextMenu* menu;

    bool _append;
    bool _stream;

    QString clickedAnchor;
signals:    

private slots:
    void lookupInDictionary();
    void copySelected();
    void enableCopy(bool);
    void saveAsHtml();
    void changeAppearance();

public slots:

};

#endif // GAMEWINDOW_H
