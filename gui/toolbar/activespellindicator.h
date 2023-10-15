#ifndef ACTIVESPELLINDICATOR_H
#define ACTIVESPELLINDICATOR_H

#include <QObject>

#include <QLabel>
#include <QHBoxLayout>

#define T_AS_W 32
#define T_AS_H 32
#define T_AS_FONT_SIZE 14

class ActiveSpellIndicator : public QObject {
    Q_OBJECT
public:
    explicit ActiveSpellIndicator(QObject *parent = 0);
    ~ActiveSpellIndicator();

    QWidget* create();

    void setScale(float scale);

    void setToolTip(QString text);
    void setText(QString text);

    QLabel* spellLabel;

private:
    QLabel* createSpellLabel();
    void setFontScale(float scale);

signals:
    
public slots:
    
};

#endif // ACTIVESPELLINDICATOR_H
