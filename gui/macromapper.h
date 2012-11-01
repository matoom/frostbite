#ifndef MACROMAPPER_H
#define MACROMAPPER_H

#include <QLineEdit>
#include <QDebug>

class MacroMapper : public virtual QLineEdit {
    Q_OBJECT

public:
    MacroMapper(QWidget *parent = 0);

private:

protected:
    virtual void keyPressEvent(QKeyEvent *event) = 0;
};

#endif // MACROMAPPER_H
