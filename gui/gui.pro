#-------------------------------------------------
#
# Project created by QtCreator 2011-12-08T17:56:21
#
#-------------------------------------------------

! include( ../common.pri ) {
    error( Couldn't find the common.pri file! )
}

QT       += core gui network xml

TARGET = FrostBite
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    clientsettings.cpp \
    connectionmanager.cpp \
    windowmanager.cpp \
    toolbarmanager.cpp \
    navigationdisplay.cpp \
    roundtimedisplay.cpp \
    gamewindow.cpp \
    genericwindow.cpp \
    vitalsindicator.cpp \
    statusindicator.cpp \
    wieldindicator.cpp \
    quickbuttondisplay.cpp \
    ping.cpp \
    commandparser.cpp \
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
    expmodel.cpp \
    gamedatacontainer.cpp \
    roommodel.cpp \
    wieldmodel.cpp \
    dataconverterservice.cpp \
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
    wizardpage.cpp

HEADERS  += mainwindow.h \
    clientsettings.h \
    connectionmanager.h \
    windowmanager.h \
    toolbarmanager.h \
    toolbardefines.h \
    navigationdisplay.h \
    navigationdefines.h \
    roundtimedisplay.h \
    gamewindow.h \
    genericwindow.h \
    vitalsindicator.h \
    statusindicator.h \
    wieldindicator.h \
    quickbuttondisplay.h \
    ping.h \
    commandparser.h \
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
    expmodel.h \
    gamedatacontainer.h \
    roommodel.h \
    wieldmodel.h \
    dataconverterservice.h \
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
    wizardpage.h

FORMS    += mainwindow.ui \
    highlightdialog.ui \
    highlightadddialog.ui \
    highlighteditdialog.ui \
    macrodialog.ui \
    quickbuttoneditdialog.ui \
    appearancedialog.ui \
    aboutdialog.ui \
    connectwizard.ui

RESOURCES += \
    toolbar.qrc \
    control.qrc \
    window.qrc

RC_FILE = resources.rc

OTHER_FILES += \
    resources.rc

LIBS += -L../gui

TARGET = ../../Frostbite
