#ifndef WIELDMODEL_H
#define WIELDMODEL_H

#include <QObject>

class WieldModel : public QObject
{
    Q_OBJECT
public:
    explicit WieldModel(QObject *parent = 0);

    QString toString();

public:
    QString getRight() { return right; }
    void setRight(QString right) { this->right = right; }

    QString getRightNoun() { return rightNoun; }
    void setRightNoun(QString rightNoun) { this->rightNoun = rightNoun; }

    QString getLeft() { return left; }
    void setLeft(QString left) { this->left = left; }

    QString getLeftNoun() { return leftNoun; }
    void setLeftNoun(QString leftNoun) { this->leftNoun = leftNoun; }

private:
    QString right;
    QString rightNoun;
    QString left;
    QString leftNoun;

signals:
    
public slots:
    
};

#endif // WIELDMODEL_H
