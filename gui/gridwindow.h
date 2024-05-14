#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QTableWidget>
#include <QDockWidget>
#include <QLabel>

class MainWindow;
class GeneralSettings;
class WindowFacade;
class ContextMenu;

class GridWindow : public QTableWidget {
    Q_OBJECT

public:
    explicit GridWindow(QString title, QWidget *parent = 0);
    ~GridWindow();

    QColor getBgColor();
    QColor getTextColor();

    QLabel* gridValueLabel(QWidget* parent);

    void track(QString skillName, QWidget* widget);  

    ContextMenu* getMenu();
    void setWindowParameter(QString parameter, QVariant value);
    QVariant getWindowParameter(QString parameter, QVariant defaultValue);

private:
    MainWindow* mainWindow;
    GeneralSettings* settings;
    WindowFacade* wm;

    QStringList tracked;
    QFont gridFont;
    QColor textColor;
    QColor backgroundColor;

    QString windowId;

    void loadSettings();    
    void invertColors(QWidget* widget);
    void setItemColors(QWidget* widget, QColor text, QColor background);

    void contextMenuEvent(QContextMenuEvent* event);
    void buildContextMenu();

    void updateFont();
    void updateSize();

    QAction* appearanceAct;
    QAction* fontAct;
    QAction* clearFontAct;
    ContextMenu* menu;

signals:

public slots:
    void addRemoveTracked(int, int);
    void updateSettings();
    void track(QString skillName);
    void clearTracked();
    void changeAppearance();
    void selectFont();
    void clearFont();
};

#endif // EXPWINDOW_H
