#ifndef HIGHLIGHTALERTTAB_H
#define HIGHLIGHTALERTTAB_H

#include <QObject>
#include <QWidget>

#include <highlightdialog.h>
#include <highlightsettings.h>
#include <highlightsettingsentry.h>
#include <audioplayer.h>

class HighlightDialog;
class HighlightSettings;
class HighlightSettingsEntry;
class AudioPlayer;

class HighlightAlertTab : public QObject {
    Q_OBJECT

public:
    HighlightAlertTab(QObject *parent = 0);
    ~HighlightAlertTab();

    void saveChanges();
    void cancelChanges();

private:
    HighlightDialog *highlightDialog;
    HighlightSettings *settings;
    AudioPlayer *audioPlayer;

    QPushButton *applyButton;
    QGroupBox* bleedingGroup;
    QPushButton* bleedingPlayButton;
    QComboBox* bleedingPlaySelect;
    QGroupBox* stunGroup;
    QPushButton* stunPlayButton;
    QComboBox* stunPlaySelect;
    QGroupBox* healthGroup;
    QPushButton* healthPlayButton;
    QComboBox* healthPlaySelect;
    QSlider* healthSlider;
    QLabel* healthSliderText;
    QGroupBox* deathGroup;
    QPushButton* deathPlayButton;
    QComboBox* deathPlaySelect;
    QGroupBox* webbedGroup;
    QPushButton* webbedPlayButton;
    QComboBox* webbedPlaySelect;

    QHash<QString, QVariant> changeList;

    void initSettings();
    void initFileSelect();

    void initBleedingSettings();
    void initStunSettings();
    void initHealthSettings();
    void initDeathSettings();
    void initWebbedSettings();

    void registerChange(QString, QVariant);


private slots:
    void healthValueUpdated(int);

    void bleedingPlay();
    void stunPlay();
    void healthPlay();
    void deathPlay();
    void webbedPlay();
    void bleedingSelected(bool);
    void bleedingFileSelected(const QString&);
    void stunSelected(bool);
    void stunFileSelected(const QString&);
    void healthSelected(bool);
    void healthFileSelected(const QString&);
    void deathSelected(bool);
    void deathFileSelected(const QString&);
    void webbedSelected(bool);
    void webbedFileSelected(const QString&);
};

#endif // HIGHLIGHTALERTTAB_H
