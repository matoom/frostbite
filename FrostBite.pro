#-------------------------------------------------
#
# Project created by QtCreator 2011-12-08T17:56:21
#
#-------------------------------------------------

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
    connectdialog.cpp \
    highlightdialog.cpp \
    highlighttexttab.cpp \
    highlightgeneraltab.cpp \
    highlightadddialog.cpp \
    highlightsettingsentry.cpp \
    highlightsettings.cpp

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
    connectdialog.h \
    highlightdialog.h \
    highlighttexttab.h \
    highlightgeneraltab.h \
    highlightadddialog.h \
    highlightsettingsentry.h \
    highlightsettings.h

FORMS    += mainwindow.ui \
    connectdialog.ui \
    highlightdialog.ui \
    highlightadddialog.ui

RESOURCES += \
    toolbar.qrc \
    control.qrc \
    window.qrc

RC_FILE = resources.rc

OTHER_FILES += \
    resources.rc




































































































































