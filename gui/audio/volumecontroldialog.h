#ifndef VOLUMECONTROLDIALOG_H
#define VOLUMECONTROLDIALOG_H

#include <QDialog>
#include <clientsettings.h>

namespace Ui {
class VolumeControlDialog;
}

class VolumeControlDialog : public QDialog {
    Q_OBJECT

public:
    explicit VolumeControlDialog(QWidget *parent = 0);
    ~VolumeControlDialog();

private:
    Ui::VolumeControlDialog *ui;

    ClientSettings* clientSettings;

    void setVolumeText(int value);
    void muteSounds(bool checked);

public slots:
    void volumeValueChanged(int);
    void mutedValueChanged(int);
    void volumeSelected();

signals:
    void volumeChanged(int);
    void volumeMuted(bool);

};

#endif // VOLUMECONTROLDIALOG_H
