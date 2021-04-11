#ifndef HYPERLINKSERVICE_H
#define HYPERLINKSERVICE_H

#include <QObject>
#include <QStringList>

class MainWindow;

class HyperlinkService : public QObject {
    Q_OBJECT
public:
    explicit HyperlinkService(QObject *parent);
    ~HyperlinkService();

    static void addLink(QString &text, const QString &pattern, const QString &command);

public slots:
    void handleUrl(const QUrl &url);     

private:
    MainWindow* mainWindow;
    void handleActionCommand(const QString& action);

    static int createLink(QString &text, const QString &command, int indexStart, QString match);
    static QString createCommand(QString text, QString command);

signals:
    void actionCommand(const QString& action);
    void actionCommands(const QStringList& actions);
};

#endif // HYPERLINKSERVICE_H
