#include "gridwindow.h"

GridWindow::GridWindow(QWidget *parent) : QTableWidget(parent) {
    mainWindow = (MainWindow*)parent;
    settings = new GeneralSettings();
    wm = mainWindow->getWindowFacade();

    this->loadSettings();

    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(addTrackedItem(int, int)));
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

QLabel* GridWindow::ceateGridItem(QWidget* parent, QString key, QString style) {
    QLabel* label = new QLabel(parent);
    label->setObjectName(key);
    label->setStyleSheet(style);
    label->setFont(settings->dockWindowFont());

    QPalette p = label->palette();
    p.setColor(QPalette::Text, settings->dockWindowFontColor());
    p.setColor(QPalette::Base, settings->dockWindowBackground());
    label->setPalette(p);

    label->setTextFormat(Qt::RichText);

    this->setFocusPolicy(Qt::NoFocus);

    return label;
}

void GridWindow::track(QString skillName, QWidget* widget) {
    if(tracked.contains(skillName)) {
        if(widget->styleSheet().isEmpty()) {
            QPalette p = widget->palette();
            QRgb rgbText = p.color(QPalette::Text).rgba()^0xffffff;
            QRgb rgbBase = p.color(QPalette::Base).rgba()^0xffffff;

            widget->setStyleSheet(QString("QLabel {background-color: %1; color: %2;}")
                             .arg(QColor(rgbBase).name(), QColor(rgbText).name()));
        }
    } else {
        widget->setStyleSheet("");
    }
}

void GridWindow::addTrackedItem(int row, int col) {
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
        w->setStyleSheet("");
    }
}
