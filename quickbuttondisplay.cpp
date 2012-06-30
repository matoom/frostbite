#include "quickbuttondisplay.h"

QuickButtonDisplay::QuickButtonDisplay(QObject *parent) : QObject(parent) {
    toolbarManager = (ToolbarManager*)parent;
}

QToolButton *QuickButtonDisplay::actionButton(const char* obName, const char* icon, const char* value) {
    QToolButton *toolButton = new QToolButton();
    toolButton->setIconSize(QSize(32, 32));
    toolButton->setObjectName(obName);
    toolButton->setIcon(QIcon(icon));
    toolButton->setText(QString(value));
    toolButton->setToolTip(QObject::tr(value));
    toolButton->setCursor(Qt::PointingHandCursor);

    /*QAction *buttonAction = new QAction(toolButton);
    buttonAction->setData(value);
    toolButton->addAction(buttonAction);*/

    connect(toolButton, SIGNAL(clicked()), toolbarManager, SLOT(quickButtonAction()));

    return toolButton;
}

QWidget *QuickButtonDisplay::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(25, 0, 25, 0);

    hLayout->addWidget(actionButton("sword", SWORD_ICO, "take my sword from my pack"));
    hLayout->addWidget(actionButton("bow", BOW_ICO, "take my bow from my pack"));
    hLayout->addWidget(actionButton("shield", SHIELD_ICO, "remove my shield"));
    hLayout->addWidget(actionButton("bag", BAG_ICO, "look in my pack"));
    hLayout->addWidget(actionButton("xsword", XSWORD_ICO, "put my sword i my pack"));
    hLayout->addWidget(actionButton("xbow", XBOW_ICO, "put my bow in my pack"));
    hLayout->addWidget(actionButton("xshield", XSHIELD_ICO, "wear my shield"));

    widget->setLayout(hLayout);

    return widget;
}
