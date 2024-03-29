#include "quickbuttondisplay.h"

#include "defaultvalues.h"
#include "toolbar/toolbardefines.h"
#include "toolbar/toolbar.h"
#include "generalsettings.h"
#include "toolbar/quickbuttondisplay.h"
#include "toolbar/quickbuttoneditdialog.h"

QuickButtonDisplay::QuickButtonDisplay(QObject *parent) : QObject(parent) {
    toolBar = (Toolbar*)parent;
    settings = GeneralSettings::getInstance();
    editDialog = new QuickButtonEditDialog();
}

void QuickButtonDisplay::reloadSettings() {
    settings = GeneralSettings::getInstance();

    sword->setText(settings->getParameter("QuickButton/sword", "").toString());
    bow->setText(settings->getParameter("QuickButton/bow", "").toString());
    shield->setText(settings->getParameter("QuickButton/shield", "").toString());
    xsword->setText(settings->getParameter("QuickButton/xsword", "").toString());
    xbow->setText(settings->getParameter("QuickButton/xbow", "").toString());
    xshield->setText(settings->getParameter("QuickButton/xshield", "").toString());
}

QToolButton* QuickButtonDisplay::actionButton(const char* objName, const char* icon, QString value) {
    QToolButton* toolButton = new QToolButton();
    toolButton->setIconSize(QSize(T_QB_W, T_QB_H));
    toolButton->setObjectName(objName);
    toolButton->setIcon(QIcon(icon));
    toolButton->setText(value);
    toolButton->setToolTip(value);
    toolButton->setCursor(Qt::PointingHandCursor);
    toolButton->setContextMenuPolicy(Qt::CustomContextMenu);

    toolButton->setStyleSheet("QToolButton {"
                              "background: #242024;"
                              "}"
                              "QToolTip {"
                              "color: #F8F8F8;"
                              "font: 11pt \"" TOOLBAR_FONT "\";"
                              "background-color: #383533;"
                              "border: 2px outset #2a82da;"
                              "padding: 2px;"
                              "}");

    connect(toolButton, SIGNAL(clicked()),
            toolBar, SLOT(quickButtonAction()));

    return toolButton;
}

void QuickButtonDisplay::setScale(float scale) {
    sword->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
    bow->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
    shield->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
    bag->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
    xsword->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
    xbow->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
    xshield->setIconSize(QSize(T_QB_W * scale, T_QB_H * scale));
}

QWidget* QuickButtonDisplay::create() {
    buttonWidget = new QWidget;
    hLayout = new QHBoxLayout(buttonWidget);
    hLayout->setContentsMargins(20, 0, 20, 0);
    hLayout->setSpacing(0);

    sword = actionButton("sword", BUTTON_AXE_ICO,
        settings->getParameter("QuickButton/sword", "").toString());
    hLayout->addWidget(sword);
    connect(sword, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(swordButtonEdit(QPoint)));

    bow = actionButton("bow", BUTTON_BOW_ICO,
        settings->getParameter("QuickButton/bow", "").toString());
    hLayout->addWidget(bow);
    connect(bow, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bowButtonEdit(QPoint)));

    shield = actionButton("shield", BUTTON_SHIELD_ICO,
        settings->getParameter("QuickButton/shield", "").toString());
    hLayout->addWidget(shield);
    connect(shield, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(shieldButtonEdit(QPoint)));

    bag = actionButton("bag", BUTTON_BAG_ICO,
        settings->getParameter("QuickButton/bag", "").toString());
    hLayout->addWidget(bag);
    connect(bag, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bagButtonEdit(QPoint)));

    xsword = actionButton("xsword", BUTTON_XAXE_ICO,
        settings->getParameter("QuickButton/xsword", "").toString());
    hLayout->addWidget(xsword);
    connect(xsword, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(xswordButtonEdit(QPoint)));

    xbow = actionButton("xbow", BUTTON_XBOW_ICO,
        settings->getParameter("QuickButton/xbow", "").toString());
    hLayout->addWidget(xbow);
    connect(xbow, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(xbowButtonEdit(QPoint)));

    xshield = actionButton("xshield", BUTTON_XSHIELD_ICO,
        settings->getParameter("QuickButton/xshield", "").toString());
    hLayout->addWidget(xshield);
    connect(xshield, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(xshieldButtonEdit(QPoint)));

    buttonWidget->setLayout(hLayout);

    return buttonWidget;
}

void QuickButtonDisplay::swordButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(sword->pos()));
    editDialog->updateButton(sword);
    editDialog->show();
}

void QuickButtonDisplay::bowButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(bow->pos()));
    editDialog->updateButton(bow);
    editDialog->show();
}

void QuickButtonDisplay::shieldButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(shield->pos()));
    editDialog->updateButton(shield);
    editDialog->show();
}

void QuickButtonDisplay::bagButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(bag->pos()));
    editDialog->updateButton(bag);
    editDialog->show();
}

void QuickButtonDisplay::xswordButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(xsword->pos()));
    editDialog->updateButton(xsword);
    editDialog->show();
}

void QuickButtonDisplay::xbowButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(xbow->pos()));
    editDialog->updateButton(xbow);
    editDialog->show();    
}

void QuickButtonDisplay::xshieldButtonEdit(const QPoint&) {
    editDialog->updateLocation(buttonWidget->mapToGlobal(xshield->pos()));
    editDialog->updateButton(xshield);
    editDialog->show();
}

QuickButtonDisplay::~QuickButtonDisplay() {
    delete sword;
    delete bow;
    delete shield;
    delete bag;
    delete xsword;
    delete xbow;
    delete xshield;
    delete hLayout;
    delete buttonWidget;
    delete editDialog;
}
