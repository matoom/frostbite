#ifndef HIGHLIGHTGENERALTAB_H
#define HIGHLIGHTGENERALTAB_H

#include <QObject>
#include <QWidget>

#include <highlightdialog.h>
#include <highlightsettings.h>
#include <audioplayer.h>

class HighlightDialog;
class HighlightSettings;
class AudioPlayer;

class HighlightGeneralTab : public QObject {
    Q_OBJECT

public:
    explicit HighlightGeneralTab(QObject *parent = 0);
    ~HighlightGeneralTab();

    void saveChanges();
    void cancelChanges();

private:
    HighlightDialog *highlightDialog;
    HighlightSettings *settings;
    AudioPlayer *audioPlayer;

    QHash<QString, QHash<QString, QVariant> > highlightList;
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
    void loadSettings();
    void prepareList();
    void updateControls(QListWidgetItem*);
    void clearControls();
    //void updateListColor(QString key);
    void registerChange(QString currentItemKey);
    void updateAlertSettings(QString key, QVariant value);
    void initFileSelect();
    void updateSelectedItemColor(QListWidgetItem *current);
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
