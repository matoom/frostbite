#include "mutebutton.h"

#include "clientsettings.h"
#include "toolbar/toolbar.h"
#include "toolbar/toolbardefines.h"
#include "defaultvalues.h"
#include "mainwindow.h"
#include "clientsettings.h"

MuteButton::MuteButton(QWidget *parent) : QToolButton(parent) {
    mainWindow = (MainWindow*)parent;
    clientSettings = ClientSettings::getInstance();

    setIconSize(QSize(T_MUTE_W, T_MUTE_H));
    setObjectName("muted");
    setCursor(Qt::PointingHandCursor);

    setStyleSheet("QToolButton {"
                  "background: #242024;"
                  "}"
                  "QToolTip {"
                  "color: #F8F8F8;"
                  "font: 11pt \"" TOOLBAR_FONT "\";"
                  "background-color: #383533;"
                  "border: 2px outset #2a82da;"
                  "padding: 2px;"
                  "}");

    this->muted = clientSettings->getParameter("Audio/muted", false).toBool();
    this->setMuted(this->muted);

    connect(this, SIGNAL(clicked()), this, SLOT(clicked()));

    connect(this, SIGNAL(volumeMuted(bool)), mainWindow, SLOT(menuVolumeMuted(bool)));
    connect(mainWindow, SIGNAL(volumeMuted(bool)), this, SLOT(setMuted(bool)));
}

void MuteButton::setScale(float scale) {
    setIconSize(QSize(T_MUTE_W * scale, T_MUTE_H * scale));
}

void MuteButton::clicked() {        
    this->muted = !this->muted;
    clientSettings->setParameter("Audio/muted", this->muted);       
    emit volumeMuted(muted);
}

void MuteButton::setMuted(bool muted) {
    if(muted) {                        
        setIcon(QIcon(BUTTON_MUTE));
        setToolTip("Unmute sounds");
    } else {
        setIcon(QIcon(BUTTON_UNMUTE));
        setToolTip("Mute sounds");
    }
}
