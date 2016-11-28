#ifndef HIGHLIGHTGENERALTAB_H
#define HIGHLIGHTGENERALTAB_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>

#include <text/highlight/highlightdialog.h>
#include <text/highlight/highlightsettings.h>
#include <audioplayer.h>
#include <defaultvalues.h>
#include <generalsettings.h>

class HighlightDialog;
class HighlightSettings;
class AudioPlayer;
class GeneralSettings;

class HighlightGeneralTab : public QObject {
    Q_OBJECT

public:
    explicit HighlightGeneralTab(QObject *parent = 0);
    ~HighlightGeneralTab();

    void saveChanges();
    void cancelChanges();

    void updateSettings();
    void loadSettings();
    void prepareList();

private:
    HighlightDialog *highlightDialog;
    HighlightSettings *settings;
    GeneralSettings* generalSettings;
    AudioPlayer *audioPlayer;

    QMap<QString, QHash<QString, QVariant> > highlightList;
    QList<QString> generalChangeList;

    QListWidget* listWidget;
    QGroupBox* alertGroup;
    QComboBox* fileSelect;
    QPushButton* playButton;
    QPushButton* applyButton;

    QAction *colorAct;
    QAction *editAct;
    QMenu *menu;

    void initContextMenu();
    void setBackground();
    void updateControls(QListWidgetItem*);
    void clearControls();
    void registerChange(QString currentItemKey);
    void updateAlertSettings(QString key, QVariant value);
    void initFileSelect();
    void updateSelectedItemColor(QListWidgetItem *current);
    void updateIcon(QListWidgetItem*, QListWidgetItem*);
    QHash<QString, QVariant> readSettings(QString id, QString name, QColor color);

signals:

private slots:
    void colorDialog();
    void playSound();
    void itemSelected(QListWidgetItem* current, QListWidgetItem* previous);
    void fileSelected(const QString& text);
    void alertClicked(bool on);
    void listWidgetMenuRequested(const QPoint &point);
};

#endif // HIGHLIGHTGENERALTAB_H
