#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T13:16:25
#
#-------------------------------------------------

!include(../common.pri) {
    error("Could not find the common.pri file!")
}

QT       -= gui

win32 {
    TARGET = ../../shared_win
}

unix {
    TARGET = ../shared_win
}

TEMPLATE = lib

DEFINES += SHARED_WIN_LIBRARY

SOURCES += shared_win.cpp

HEADERS += shared_win.h\
        shared_win_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1A27C64
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = shared_win.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
