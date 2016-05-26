#-------------------------------------------------
#
# Project created by QtCreator 2011-12-08T17:56:21
#
#-------------------------------------------------

!include(../common.pri) {
    error("Could not find the common.pri file!")
}

greaterThan(QT_VERSION, 5){
    QT       += widgets core gui network xml multimedia concurrent
} else {
    QT       += core gui network xml
}

include(../log4qt/src/log4qt/log4qt.pri)

#https://github.com/qtproject/qt-solutions
include(../singleapp/qtsingleapplication.pri)

greaterThan(QT_VERSION, 5){
   include(../cleanlooks/cleanlooks.pri)
}

include(maps/maps.pri)

win32 {
    TARGET = ../../FrostBite
}

unix {
    TARGET = ../FrostBite
}

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    clientsettings.cpp \
    toolbar.cpp \
    navigationdisplay.cpp \
    roundtimedisplay.cpp \
    gamewindow.cpp \
    vitalsindicator.cpp \
    statusindicator.cpp \
    wieldindicator.cpp \
    quickbuttondisplay.cpp \
    commandline.cpp \
    keyboardfilter.cpp \
    menuhandler.cpp \
    highlightdialog.cpp \
    highlighttexttab.cpp \
    highlightgeneraltab.cpp \
    highlightadddialog.cpp \
    highlightsettingsentry.cpp \
    highlightsettings.cpp \
    highlighteditdialog.cpp \
    highlightalerttab.cpp \
    audioplayer.cpp \
    eauthservice.cpp \
    script.cpp \
    scriptservice.cpp \
    gamedatacontainer.cpp \
    highlighter.cpp \
    timerbar.cpp \
    macrodialog.cpp \
    macrosettings.cpp \
    macrothread.cpp \
    macroservice.cpp \
    spellindicator.cpp \
    wordcompleter.cpp \
    quickbuttoneditdialog.cpp \
    appearancedialog.cpp \
    aboutdialog.cpp \
    connectwizard.cpp \
    wizardpage.cpp \
    scripteditdialog.cpp \
    genericwindowfactory.cpp \
    genericwindow.cpp \
    fullscreenbutton.cpp \
    profileadddialog.cpp \
    generalsettings.cpp \
    highlighterthread.cpp \
    scriptwriterthread.cpp \
    mainlogger.cpp \
    thoughtslogger.cpp \
    deathslogger.cpp \
    conversationslogger.cpp \
    arrivalslogger.cpp \
    debuglogger.cpp \
    snapshot.cpp \
    guiapplication.cpp \
    tray.cpp \
    activespellindicator.cpp \
    tcpclient.cpp \
    windowfacade.cpp \
    textutils.cpp \
    xmlparserthread.cpp \
    shareddataservice.cpp \
    gridwindowfactory.cpp \
    gridwindow.cpp \
    gridhighlighterthread.cpp \
    scriptapiserver.cpp \
    apisettings.cpp \
    authlogger.cpp

HEADERS  += mainwindow.h \
    clientsettings.h \
    toolbar.h \
    toolbardefines.h \
    navigationdisplay.h \
    navigationdefines.h \
    roundtimedisplay.h \
    gamewindow.h \
    vitalsindicator.h \
    statusindicator.h \
    wieldindicator.h \
    quickbuttondisplay.h \
    commandline.h \
    version.h \
    keyboardfilter.h \
    menuhandler.h \
    highlightdialog.h \
    highlighttexttab.h \
    highlightgeneraltab.h \
    highlightadddialog.h \
    highlightsettingsentry.h \
    highlightsettings.h \
    highlighteditdialog.h \
    highlightalerttab.h \
    audioplayer.h \
    eauthservice.h \
    script.h \
    scriptservice.h \
    gamedatacontainer.h \
    highlighter.h \
    timerbar.h \
    macrodialog.h \
    macrosettings.h \
    macrothread.h \
    macroservice.h \
    defaultvalues.h \
    spellindicator.h \
    environment.h \
    wordcompleter.h \
    quickbuttoneditdialog.h \
    appearancedialog.h \
    aboutdialog.h \
    connectwizard.h \
    wizardpage.h \
    scripteditdialog.h \
    genericwindowfactory.h \
    genericwindow.h \
    fullscreenbutton.h \
    profileadddialog.h \
    generalsettings.h \
    highlighterthread.h \
    scriptwriterthread.h \
    globaldefines.h \
    mainlogger.h \
    thoughtslogger.h \
    deathslogger.h \
    conversationslogger.h \
    arrivalslogger.h \
    debuglogger.h \
    snapshot.h \
    windowinterface.h \
    guiapplication.h \
    tray.h \
    activespellindicator.h \
    tcpclient.h \
    windowfacade.h \
    textutils.h \
    xmlparserthread.h \
    shareddataservice.h \
    gridwindowfactory.h \
    gridwindow.h \
    gridhighlighterthread.h \
    scriptapiserver.h \
    apisettings.h \
    authlogger.h

FORMS    += mainwindow.ui \
    highlightdialog.ui \
    highlightadddialog.ui \
    highlighteditdialog.ui \
    macrodialog.ui \
    quickbuttoneditdialog.ui \
    appearancedialog.ui \
    aboutdialog.ui \
    connectwizard.ui \
    scripteditdialog.ui \
    profileadddialog.ui

RESOURCES += \
    toolbar.qrc \
    control.qrc \
    window.qrc

RC_FILE = resources.rc

OTHER_FILES += \
    resources.rc

# use relative path for libs in unix
unix:!mac{
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
}

ICON = images/shield.icns

#profile
#custom step: gcc -pg main.cpp -o profile_main

#debug flags
#CONFIG += DEBUG
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_CXXFLAGS_MT_DBG += -pg
#QMAKE_LFLAGS_DEBUG += -pg




