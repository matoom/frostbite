#ifndef GENERICWINDOW_H
#define GENERICWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QMouseEvent>

#include <generalsettings.h>
#include <dict/dictionarysettings.h>
#include <mainwindow.h>
#include <windowinterface.h>
#include <snapshot.h>
#include <QScrollBar>
#include <custom/contextmenu.h>

class MainWindow;
class WindowFacade;
class GeneralSettings;
class Snapshot;

class GenericWindow : public QPlainTextEdit, public WindowInterface {
    Q_OBJECT

public:
    explicit GenericWindow(QString title, QWidget *parent = 0);
    ~GenericWindow();

    QColor getBgColor();
    QColor getTextColor();
    QTextDocument* getDocument();
    QString getObjectName();
    QPlainTextEdit* getMainWindow();

    void setAppend(bool);
    bool append();

    void setStream(bool);
    bool stream();

private:
    void contextMenuEvent(QContextMenuEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *e);    
    void buildContextMenu();    
    void loadSettings();

    MainWindow* mainWindow;
    GeneralSettings* settings;
    DictionarySettings* dictionarySettings;
    WindowFacade* wm;
    QString windowId;

    Snapshot* snapshot;

    QAction* appearanceAct;
    QAction* lookupDictAct;    
    QAction* copyAct;
    QAction* selectAct;
    QAction* saveAct;
    QAction* clearAct;
    QAction* fontAct;
    QAction* clearFontAct;
    ContextMenu* menu;

    bool _append;
    bool _stream;

signals:

private slots:
    void lookupInDictionary();    
    void copySelected();
    void enableCopy(bool);
    void saveAsHtml();
    void changeAppearance();
    void selectFont();
    void clearFont();    
    void updateSettings();
    
public slots:
    void appendHtmlStream(const QString&);

};

#endif // GENERICWINDOW_H
