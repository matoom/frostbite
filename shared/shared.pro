#-------------------------------------------------
#
# Project created by QtCreator 2014-03-06T16:07:45
#
#-------------------------------------------------

# use mingw gcc 4.6.2 to compile
# bug in boost shared memory gcc for 4.4.0 -
# win32_threads.c:1246: internal compiler error:
# in gimple_rhs_has_side_effects, at gimple.c:2343

!include(../common.pri) {
    error("Could not find the common.pri file!")
}

QT       -= gui

win32 {
    TARGET = ../../shared
}

unix {
    TARGET = ../shared
}

# use relative path for libs in unix
unix:!mac{
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
}

TEMPLATE = lib

DEFINES += DATA_LIBRARY

SOURCES += shared.cpp

HEADERS += shared.h\
        shared_global.h \
    defines.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE26F0066
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = shared.dll
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

