#ifndef APPEARANCEDIALOG_H
#define APPEARANCEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QToolButton>
#include <QMenu>
#include <QLabel>
#include <QColorDialog>
#include <QFontDialog>

#include <mainwindow.h>
#include <defaultvalues.h>
#include <generalsettings.h>
#include <windowmanager.h>

class MainWindow;
class GeneralSettings;
class WindowManager;

namespace Ui {
    class AppearanceDialog;
}

class AppearanceDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit AppearanceDialog(QWidget *parent = 0);
    ~AppearanceDialog();

    void loadSettings();
    void updateSettings();
    
private:    
    Ui::AppearanceDialog *ui;
    MainWindow* mainWindow;
    WindowManager* windowManager;
    GeneralSettings* settings;

    QToolButton* mainBgSelect;
    QToolButton* mainFontSelect;
    QToolButton* mainFontColorSelect;

    QToolButton* dockBgSelect;
    QToolButton* dockFontSelect;
    QToolButton* dockFontColorSelect;

    QFont mainFontValue;
    QColor mainFontColorValue;
    QColor mainBackgroundValue;

    QFont dockFontValue;
    QColor dockFontColorValue;
    QColor dockBackgroundValue;

    void populateMainBox();
    void populateDockBox();

    void reset();

    QHash<QString, QVariant> changeList;
    QHash<QString, QColor> colorChangeList;
    QHash<QString, QFont> fontChangeList;

    QToolButton* selectButton(int, int, QString);
    QLabel* label(QString);
    void setSelectBackground(QString, QToolButton*, QString);
    void setSelectFont(QString, QToolButton*, QFont);

private slots:
    void selectMainBg();
    void selectMainFont();
    void selectMainFontColor();
    void selectDockBg();
    void selectDockFont();
    void selectDockFontColor();

    void okPressed();
    void applyPressed();
    void cancelPressed();

    void cancelChanges();
    void saveChanges();
};

#endif // APPEARANCEDIALOG_H
