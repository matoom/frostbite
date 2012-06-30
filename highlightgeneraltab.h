#ifndef HIGHLIGHTGENERALTAB_H
#define HIGHLIGHTGENERALTAB_H

#include <QObject>
#include <QWidget>

#include <highlightdialog.h>
#include <clientsettings.h>

class HighlightDialog;

class HighlightGeneralTab : public QObject {
    Q_OBJECT

public:
    explicit HighlightGeneralTab(QObject *parent = 0);
    ~HighlightGeneralTab();

    void saveChanges();
    void cancelChanges();

private:
    HighlightDialog *highlightDialog;
    ClientSettings *settings;

    QHash<QString, QHash<QString, QVariant> > highlightList;
    QList<QString> generalChangeList;

    QListWidget* listWidget;
    QGroupBox* alertGroup;
    //QLineEdit* delayLine;
    QComboBox* fileSelect;
    QPushButton* applyButton;

    QAction *colorAct;
    QAction *editAct;
    QMenu *menu;

    void initContextMenu();
    void loadSettings();
    void prepareList();
    void updateControls(QString key);
    void updateListColor(QString key);
    void registerChange(QString currentItemKey);
    void updateAlertSettings(QString key, QVariant value);
    void readSoundFiles();
    void updateSelectedItemColor(QListWidgetItem *current);
    QHash<QString, QVariant> readSettings(QString id, QString name, QColor color);

signals:

private slots:
    void colorDialog();
    void itemSelected(QListWidgetItem* current, QListWidgetItem* previous);
    void fileSelected(const QString& text);
    void alertClicked(bool on);
    //void delayUpdated(const QString& text);
    void listWidgetMenuRequested(const QPoint &point);
};

#endif // HIGHLIGHTGENERALTAB_H
