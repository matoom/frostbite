#include "wizardpage.h"

WizardPage::WizardPage(QWidget *parent) : QWizardPage(parent) {
    /* this class is promoted to ConnectWizard pages to expose
       registerField function from protected scope */
}

void WizardPage::registerField(const QString &name, QWidget *widget, const char *property, const char *changedSignal) {
    QWizardPage::registerField(name, widget, property, changedSignal);
}
