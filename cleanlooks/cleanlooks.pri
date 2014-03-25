INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/qcleanlooksstyle.h 

SOURCES += \
    $$PWD/qcleanlooksstyle.cpp \
    $$PWD/plugin.cpp
    
OTHER_FILES += \
    $$PWD/cleanlooks.json

include(shared/shared.pri)

