QT += testlib xml gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

TARGET = testxml

INCLUDEPATH += $$PWD/../gui
DEPENDPATH += $$PWD/../gui

# Test
SOURCES +=  tst_xml.cpp

# Test dependencies

include(../gui/xml/xml.pri)

SOURCES += \
    $$PWD/../gui/textutils.cpp \
    $$PWD/../gui/gamedatacontainer.cpp \
    $$PWD/../gui/hyperlinkutils.cpp

HEADERS += \
    $$PWD/../gui/textutils.h \
    $$PWD/../gui/gamedatacontainer.h \
    $$PWD/../gui/hyperlinkutils.h
