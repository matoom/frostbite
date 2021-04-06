#ifndef HYPERLINKSERVICE_H
#define HYPERLINKSERVICE_H

#include <QObject>
#include <QStringList>

class HyperlinkService : public QObject {
    Q_OBJECT
public:
    explicit HyperlinkService(QObject *parent);
    ~HyperlinkService();
public slots:
    void handleUrl(const QUrl &url);
private:
    void handleActionCommand(const QString& action);
signals:
    void actionCommand(const QString& action);
    void actionCommands(const QStringList& actions);
};

#endif // HYPERLINKSERVICE_H
