#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QPlainTextEdit>
#include <QMouseEvent>
#include <QResizeEvent>

#include "windowinterface.h"

class MainWindow;
class WindowFacade;
class Snapshot;
class Compass;
class GeneralSettings;
class DictionarySettings;
class ContextMenu;

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
    void mouseMoveEvent(QMouseEvent*);
    
    void resetWindow();

    void loadSettings();
    void buildContextMenu();

    void showEvent(QShowEvent*);
    bool event(QEvent *event);
    
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GeneralSettings* settings;
    DictionarySettings* dictionarySettings;

    Snapshot* snapshot;

    QAction* appearanceAct;
    QAction* lookupDictAct;
    QAction* lookupWikiAct;
    QAction* copyAct;
    QAction* selectAct;
    QAction* clearAct;
    QAction* saveAct;
    QAction* distractionFreeModeAct;
    ContextMenu* menu;

    bool _append;
    bool _stream;

    QString clickedAnchor;

    struct DictionaryEvent {
        QString word;
        QPoint point;
        bool active;
    };

    DictionaryEvent currentDictEvent;
    
signals:

private slots:
    void lookupInDictionary();
    void lookupInElanthipedia();
    void copySelected();
    void enableCopy(bool);
    void saveAsHtml();
    void changeAppearance();
    void translationFinished(QString word, QString translation);
    void translationFailed(QString reason);

public slots:

};

#endif // GAMEWINDOW_H
