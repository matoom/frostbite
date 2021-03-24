#ifndef DICTIONARYDIALOG_H
#define DICTIONARYDIALOG_H

#include <QDialog>

class QLineEdit;
class DictionarySettings;

class DictionaryDialog : public QDialog {
    Q_OBJECT
public:
    explicit DictionaryDialog(QWidget *parent = 0);
    ~DictionaryDialog();

private:
    void saveChanges();
    void loadSettings();

private slots:
    void okPressed();
    void cancelPressed();


private:
    QLineEdit* dictNameEdit;
    QLineEdit* dictArgumentsEdit;

    DictionarySettings* settings;
};

#endif // DICTIONARYDIALOG_H
