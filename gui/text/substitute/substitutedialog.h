#ifndef SUBSTITUTEDIALOG_H
#define SUBSTITUTEDIALOG_H

#include <QDialog>

namespace Ui {
    class SubstituteDialog;
}

class SubstituteDialog : public QDialog {
    Q_OBJECT

public:
    explicit SubstituteDialog(QWidget *parent = 0);
    ~SubstituteDialog();

private:
    Ui::SubstituteDialog *ui;
};

#endif // SUBSTITUTEDIALOG_H
