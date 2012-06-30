#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

#include <mainwindow.h>
#include <clientsettings.h>

class MainWindow;
class ClientSettings;

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog {
    Q_OBJECT

public:
    ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private:
    Ui::ConnectDialog *ui;

    MainWindow *mainWindow;    
    ClientSettings *settings;

    void closeEvent(QCloseEvent *event);
    void loadData();
    void proxyEnabled(bool state);
    void saveSettings();

private slots:
    void proxyState(int state);
    void connectStart();
    void connectCancel();
};

#endif // CONNECTDIALOG_H
