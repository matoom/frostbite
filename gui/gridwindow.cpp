#include "gridwindow.h"

GridWindow::GridWindow(QWidget *parent) : QTableWidget(parent) {
    mainWindow = (MainWindow*)parent;
    settings = GeneralSettings::getInstance();
    wm = mainWindow->getWindowFacade();

    this->loadSettings();

    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(addRemoveTracked(int, int)));
    connect(this->verticalHeader(), SIGNAL(sectionCountChanged(int, int)), this, SLOT(resize(int, int)));

    this->setFocusPolicy(Qt::NoFocus);
}

void GridWindow::updateSettings() {    
    settings = GeneralSettings::getInstance();
    this->loadSettings();
    this->clearTracked();
}

void GridWindow::loadSettings() {    
    font = settings->dockWindowFont();
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);
    textColor = settings->dockWindowFontColor();
    backgroundColor = settings->dockWindowBackground();
}

void GridWindow::resize(int, int) {
    this->resizeRowsToContents();
    int height = this->verticalHeader()->fontMetrics().height();
    this->verticalHeader()->setDefaultSectionSize(height + 4);
}

QColor GridWindow::getBgColor() {
    return viewport()->palette().color(QPalette::Base);
}

QColor GridWindow::getTextColor() {
    return viewport()->palette().color(QPalette::Text);
}

QLabel* GridWindow::gridValueLabel(QWidget* parent) {
    QLabel* label = new QLabel(parent);    
    label->setFont(font);

    QPalette p = label->palette();
    p.setColor(QPalette::Text, textColor);
    p.setColor(QPalette::Base, backgroundColor);
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

void GridWindow::setItemColors(QWidget* widget, QColor text, QColor background) {
    QPalette p = widget->palette();
    p.setColor(QPalette::Text, text);
    p.setColor(QPalette::Base, background);
    widget->setPalette(p);
}

void GridWindow::track(QString skillName) {
    if(!tracked.contains(skillName))
        tracked << skillName;

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
        if(widget->property("tracked") == 0) {
            this->setItemColors(widget, backgroundColor, textColor);
            widget->setProperty("tracked", 1);
        }
    } else {
        if(widget->property("tracked") == 1) {
            this->setItemColors(widget, textColor, backgroundColor);
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
        this->setItemColors(w, textColor, backgroundColor);
        w->setProperty("tracked", 0);
    }
}

GridWindow::~GridWindow() {
}
