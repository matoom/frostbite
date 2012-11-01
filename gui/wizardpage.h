#ifndef WIZARDPAGE_H
#define WIZARDPAGE_H

#include <QWizardPage>

class WizardPage: public QWizardPage {

    public:
        WizardPage( QWidget* parent = 0 );
        void registerField(const QString &name, QWidget *widget, const char *property = 0, const char *changedSignal = 0);
};

#endif // WIZARDPAGE_H
