#ifndef APPEARANCEDIALOG_H
#define APPEARANCEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QToolButton>
#include <QMenu>
#include <QLabel>
#include <QColorDialog>
#include <QFontDialog>

class MainWindow;
class GeneralSettings;
class WindowFacade;
class RoundTimeDisplay;
class CommandLine;

namespace Ui {
    class AppearanceDialog;
}

class AppearanceDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit AppearanceDialog(QWidget *parent = 0);
    ~AppearanceDialog();

    void loadSettings();
    
private:    
    Ui::AppearanceDialog *ui;
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    CommandLine* commandLine;
    GeneralSettings* settings;

    QToolButton* mainBgSelect;
    QToolButton* mainFontSelect;
    QToolButton* mainFontColorSelect;

    QToolButton* dockBgSelect;
    QToolButton* dockFontSelect;
    QToolButton* dockFontColorSelect;

    QToolButton* cmdBgSelect;
    QToolButton* cmdFontSelect;
    QToolButton* cmdFontColorSelect;

    QToolButton* cmdRtColorSelect;
    QToolButton* cmdCtColorSelect;

    QFont mainFontValue;
    QColor mainFontColorValue;
    QColor mainBackgroundValue;

    QFont dockFontValue;
    QColor dockFontColorValue;
    QColor dockBackgroundValue;

    QFont cmdFontValue;
    QColor cmdFontColorValue;
    QColor cmdBackgroundValue;

    QColor cmdRtColorValue;
    QColor cmdCtColorValue;

    void populateMainBox();
    void populateDockBox();
    void populateCmdBox();

    void reset();

    QHash<QString, QVariant> changeList;
    QHash<QString, QColor> colorChangeList;
    QHash<QString, QFont> fontChangeList;

    QToolButton* selectButton(int, int, QString);
    QLabel* label(QString);
    void setSelectBackground(QString, QToolButton*, QString);
    void setSelectFont(QString, QToolButton*, QFont);

    RoundTimeDisplay* roundTimeDisplay;

public slots:
    void reloadSettings();

private slots:
    void selectMainBg();
    void selectMainFont();
    void selectMainFontColor();

    void selectDockBg();
    void selectDockFont();
    void selectDockFontColor();

    void selectCmdBg();
    void selectCmdFont();
    void selectCmdFontColor();

    void selectCmdRtColor();
    void selectCmdCtColor();

    void okPressed();
    void applyPressed();
    void cancelPressed();

    void cancelChanges();
    void saveChanges();
};

#endif // APPEARANCEDIALOG_H
