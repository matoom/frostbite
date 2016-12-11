#include "gridwindow.h"

GridWindow::GridWindow(QWidget *parent) : QTableWidget(parent) {
    mainWindow = (MainWindow*)parent;
    settings = new GeneralSettings();
    wm = mainWindow->getWindowFacade();

    this->loadSettings();

    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(addRemoveTracked(int, int)));

    this->setFocusPolicy(Qt::NoFocus);
}

void GridWindow::updateSettings() {
    delete settings;
    settings = new GeneralSettings();
}

void GridWindow::loadSettings() {
    QFont font = settings->dockWindowFont();
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);
}

QColor GridWindow::getBgColor() {
    return viewport()->palette().color(QPalette::Base);
}

QColor GridWindow::getTextColor() {
    return viewport()->palette().color(QPalette::Text);
}

QLabel* GridWindow::gridValueLabel(QWidget* parent, GeneralSettings* settings, QString key) {
    QLabel* label = new QLabel(parent);
    label->setObjectName(key);
    label->setFont(settings->dockWindowFont());

    QPalette p = label->palette();
    p.setColor(QPalette::Text, settings->dockWindowFontColor());
    p.setColor(QPalette::Base, settings->dockWindowBackground());
    label->setPalette(p);

    label->setTextFormat(Qt::RichText);
    label->setAutoFillBackground(true);
    label->setProperty("tracked", 0);
    return label;
}

void GridWindow::invertColors(QWidget* widget) {
    QPalette p = widget->palette();
    QRgb rgbText = p.color(QPalette::Text).rgba()^0xffffff;
    QRgb rgbBase = p.color(QPalette::Base).rgba()^0xffffff;

    p.setColor(QPalette::Text, QColor(rgbText));
    p.setColor(QPalette::Base, QColor(rgbBase));

    widget->setPalette(p);
}

void GridWindow::track(QString skillName) {
    if(!tracked.contains(skillName)) {
        tracked << skillName;
    }

    int rows = this->rowCount();

    QRegExp rx(skillName);
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    for(int i = 0; i < rows; i++) {
        QWidget* widget = this->cellWidget(i, 0);
        if(widget != NULL) {
            if(rx.exactMatch(widget->objectName())) track(skillName, widget);
        }
    }
}

void GridWindow::track(QString skillName, QWidget* widget) {    
    if(tracked.contains(skillName)) {
        if(widget->property("tracked") == QVariant::Invalid ||
                widget->property("tracked") == 0) {
            this->invertColors(widget);
            widget->setProperty("tracked", 1);
        }
    } else {
        if(widget->property("tracked") == 1) {
            this->invertColors(widget);
            widget->setProperty("tracked", 0);
        }
    }
}

void GridWindow::addRemoveTracked(int row, int col) {
    QWidget* w = this->cellWidget(row, col);
    if(!tracked.contains(w->objectName())) {
        tracked << w->objectName();
    } else {
        tracked.removeAll(w->objectName());
    }
    this->track(w->objectName(), w);
}

void GridWindow::clearTracked() {
    tracked.clear();
    for (int i = 0; i < this->rowCount(); ++i) {
        QWidget* w = this->cellWidget(i, 0);
        track(w->objectName(), w);
    }
}
