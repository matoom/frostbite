!include(../common.pri) {
    error("Could not find the common.pri file!")
}

TARGET = ../../qcleanlooksstyle

PLUGIN_TYPE = styles

QT = core gui widgets

HEADERS += qcleanlooksstyle.h
SOURCES += qcleanlooksstyle.cpp
SOURCES += plugin.cpp

include(shared/shared.pri)

OTHER_FILES += cleanlooks.json
