#ifndef DICTIONARYDIALOG_H
#define DICTIONARYDIALOG_H

#include <QDialog>
#include <vector>
#include <utility>

class QRadioButton;
class QLineEdit;
class DictionarySettings;
class QGroupBox;

class DictionaryDialog : public QDialog {
    Q_OBJECT
public:
    typedef std::pair<Qt::KeyboardModifier, QRadioButton*> ButtonPair;
    
public:
    explicit DictionaryDialog(QWidget *parent = 0);
    ~DictionaryDialog();

private:
    void saveSettings();
    void loadSettings();

private slots:
    void okPressed();
    void cancelPressed();
    void onToggled(bool checked);

private:
    QLineEdit* dictNameEdit;
    QLineEdit* dictArgumentsEdit;
    QGroupBox* outputOptionsBox;
    QRadioButton *dictionaryWinBtn;
    QRadioButton *toolTipBtn;
    QGroupBox* hotkeyOptionsBox;
    
    DictionarySettings* settings;
    typedef std::vector<ButtonPair> ButtonVector;
    ButtonVector dblClkButtons;
};

#endif // DICTIONARYDIALOG_H
