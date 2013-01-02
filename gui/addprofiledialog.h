#ifndef ADDPROFILEDIALOG_H
#define ADDPROFILEDIALOG_H

#include <QDialog>

namespace Ui {
class AddProfileDialog;
}

class AddProfileDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddProfileDialog(QWidget *parent = 0);
    ~AddProfileDialog();
    
private:
    Ui::AddProfileDialog *ui;
};

#endif // ADDPROFILEDIALOG_H
