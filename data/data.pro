#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T13:16:25
#
#-------------------------------------------------

! include( ../common.pri ) {
    error( Couldn't find the common.pri file! )
}

QT       -= gui

TARGET = ../../data
TEMPLATE = lib

DEFINES += DATA_LIBRARY

SOURCES += data.cpp

HEADERS += data.h\
        Data_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1A27C64
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Data.dll
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
