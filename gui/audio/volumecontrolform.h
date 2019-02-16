#ifndef VOLUMECONTROLFORM_H
#define VOLUMECONTROLFORM_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class VolumeControlForm;
}

class VolumeControlForm : public QWidget{
    Q_OBJECT

public:
    explicit VolumeControlForm(QWidget *parent = 0);
    ~VolumeControlForm();

    void focusOutEvent(QFocusEvent* event);

private:
    Ui::VolumeControlForm *ui;
};

#endif // VOLUMECONTROLFORM_H
