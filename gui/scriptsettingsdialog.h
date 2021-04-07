#ifndef SCRIPTSETTINGSDIALOG_H
#define SCRIPTSETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>

class MainWindow;
class ClientSettings;

namespace Ui {
    class ScriptSettingsDialog;
}

class ScriptSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScriptSettingsDialog(QWidget *parent = 0);
    ~ScriptSettingsDialog();

private:    
    void loadSettings();

    void saveChanges();
    void cancelChanges();

    Ui::ScriptSettingsDialog *ui;

    MainWindow* mainWindow;
    ClientSettings* settings;

    QHash<QString, QVariant> changeList;

signals:
    void settingsChanged();

private slots:
    void browse();
    void inputChanged();
    void okPressed();
    void applyPressed();
    void cancelPressed();
    void inputEdited(QString);
};

#endif // SCRIPTSETTINGSDIALOG_H
