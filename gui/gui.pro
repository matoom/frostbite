#-------------------------------------------------
#
# Project created by QtCreator 2011-12-08T17:56:21
#
#-------------------------------------------------

RELEASE_VERSION = 1.17.0b

DEFINES += RELEASE_VERSION=\\\"$$RELEASE_VERSION\\\"

!include(../common.pri) {
    error("Could not find the common.pri file!")
}

QT       += widgets core gui network xml multimedia concurrent

include(../log4qt/src/log4qt/log4qt.pri)

#https://github.com/qtproject/qt-solutions
include(../singleapp/qtsingleapplication.pri)

include(../cleanlooks/cleanlooks.pri)

include(maps/maps.pri)
include(text/text.pri)
include(xml/xml.pri)
include(lich/lich.pri)
include(compass/compass.pri)
include(window/window.pri)
include(custom/custom.pri)
include(audio/audio.pri)
include(toolbar/toolbar.pri)
include(dict/dict.pri)

APP_NAME = Frostbite

win32 {
    TARGET = ../../$$APP_NAME
}

unix {
    TARGET = ../$$APP_NAME
}

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    clientsettings.cpp \
    roundtimedisplay.cpp \
    gamewindow.cpp \    
    commandline.cpp \
    keyboardfilter.cpp \
    menuhandler.cpp \
    eauthservice.cpp \
    script.cpp \
    scriptservice.cpp \
    gamedatacontainer.cpp \
    timerbar.cpp \
    macrodialog.cpp \
    macrosettings.cpp \
    macrothread.cpp \
    macroservice.cpp \   
    wordcompleter.cpp \    
    appearancedialog.cpp \
    aboutdialog.cpp \
    connectwizard.cpp \
    wizardpage.cpp \
    scripteditdialog.cpp \
    genericwindowfactory.cpp \
    genericwindow.cpp \
    profileadddialog.cpp \
    generalsettings.cpp \
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
    tcpclient.cpp \
    windowfacade.cpp \
    textutils.cpp \
    shareddataservice.cpp \
    gridwindowfactory.cpp \
    gridwindow.cpp \
    scriptapiserver.cpp \
    apisettings.cpp \
    authlogger.cpp \
    windowwriterthread.cpp \
    gridwriterthread.cpp \
    vitalsbar.cpp \
    scriptsettingsdialog.cpp \
    compass.cpp \
    genieutils.cpp \
    hyperlinkservice.cpp \
    hyperlinkutils.cpp \
    session.cpp \
    scriptstreamserver.cpp

HEADERS  += mainwindow.h \
    clientsettings.h \
    navigationdefines.h \
    roundtimedisplay.h \
    gamewindow.h \
    commandline.h \
    version.h \
    keyboardfilter.h \
    menuhandler.h \
    eauthservice.h \
    script.h \
    scriptservice.h \
    gamedatacontainer.h \
    timerbar.h \
    macrodialog.h \
    macrosettings.h \
    macrothread.h \
    macroservice.h \
    defaultvalues.h \
    environment.h \
    wordcompleter.h \
    appearancedialog.h \
    aboutdialog.h \
    connectwizard.h \
    wizardpage.h \
    scripteditdialog.h \
    genericwindowfactory.h \
    genericwindow.h \
    profileadddialog.h \
    generalsettings.h \
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
    tcpclient.h \
    windowfacade.h \
    textutils.h \
    shareddataservice.h \
    gridwindowfactory.h \
    gridwindow.h \
    scriptapiserver.h \
    apisettings.h \
    authlogger.h \
    windowwriterthread.h \
    gridwriterthread.h \
    vitalsbar.h \
    scriptsettingsdialog.h \
    compass.h \
    genieutils.h \
    hyperlinkservice.h \
    hyperlinkutils.h \
    concurrentqueue.h \
    workqueuethread.h \
    session.h \
    scriptstreamserver.h

FORMS    += mainwindow.ui \
    macrodialog.ui \
    appearancedialog.ui \
    aboutdialog.ui \
    connectwizard.ui \
    scripteditdialog.ui \
    profileadddialog.ui \
    scriptsettingsdialog.ui

RESOURCES += \
    toolbar.qrc \
    control.qrc \
    window.qrc

RC_FILE = resources.rc

OTHER_FILES += \
    resources.rc

# use relative path for libs in unix
unix:!mac{  
    QMAKE_LFLAGS += -rdynamic "-Wl,-rpath,\'\$$ORIGIN/lib\'"
}

ICON = images/shield.icns

#profile
#custom step: gcc -pg main.cpp -o profile_main

#debug flags
#CONFIG += DEBUG
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_CXXFLAGS_MT_DBG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

# automated release packaging
CONFIG(release, debug|release) {

    win32 {
        #QT 5.12.12
        # OpenSSL deployment - qt intallation specific, sensitive to build environment changes
        OPEN_SSL_DIR = $$shell_quote($$shell_path($$[QT_INSTALL_DATA]/../../Tools/QtCreator/bin))
        LIB_CRYPTO = $$shell_quote($$shell_path($$OPEN_SSL_DIR/libcrypto-1_1-x64.dll))
        LIB_SSL = $$shell_quote($$shell_path($$OPEN_SSL_DIR/libssl-1_1-x64.dll))

        DEPLOY_FILES = $$shell_quote($$shell_path($$PWD/../deploy/common))
        DEPLOY_FILES_WIN = $$shell_quote($$shell_path($$PWD/../deploy/win))
        RELEASE_PATH = $$shell_quote($$shell_path($$OUT_PWD/../release/Frostbite-$$RELEASE_VERSION))
        PLUGINS_PATH = $$shell_quote($$shell_path($$RELEASE_PATH/plugins))
        BIN = $$shell_quote($$shell_path($$OUT_PWD/../Frostbite.exe))
        DEPLOY_QT = $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/windeployqt))

        postbuild.commands = $(COPY_DIR) $$DEPLOY_FILES $$RELEASE_PATH &
        postbuild.commands += $(COPY_DIR) $$DEPLOY_FILES_WIN $$RELEASE_PATH &
        postbuild.commands += $(COPY_FILE) $$LIB_CRYPTO $$RELEASE_PATH &
        postbuild.commands += $(COPY_FILE) $$LIB_SSL $$RELEASE_PATH

        DEPLOY = $(COPY_FILE) $$BIN $$RELEASE_PATH & $$DEPLOY_QT $$RELEASE_PATH --no-translations --no-webkit2 --no-opengl-sw --no-angle --plugindir $$PLUGINS_PATH
    }

    macx {
        #QT 5.12
        APP_PATH = ../Frostbite.app
        DEPLOY_FILES = $$shell_quote($$shell_path($$PWD/../deploy/common/))
        DEPLOY_FILES_MAC = $$shell_quote($$shell_path($$PWD/../deploy/mac/))
        BIN = $$shell_quote($$shell_path($$OUT_PWD/$$APP_PATH))
        DMG = $$shell_quote($$shell_path($$OUT_PWD/../Frostbite.dmg))
        RELEASE_FILE = $$shell_quote($$shell_path($$OUT_PWD/../frostbite-osx.dmg))
        DEPLOY_PATH = $$shell_quote($$shell_path($$BIN/Contents/MacOS))
        DEPLOY_QT = $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/macdeployqt))

        postbuild.commands = $(COPY_DIR) $$DEPLOY_FILES $$DEPLOY_PATH &&
        postbuild.commands += $(COPY_DIR) $$DEPLOY_FILES_MAC $$DEPLOY_PATH

        DEPLOY = $$DEPLOY_QT $$APP_PATH -dmg && $(COPY_FILE) $$DMG $$RELEASE_FILE
    }

    unix:!macx {
        # no deployer for linux; copy libs/plugins
        BIN = $$shell_quote($$shell_path($$OUT_PWD/../$$APP_NAME))
        DEPLOY_FILES = $$shell_quote($$shell_path($$PWD/../deploy/common/.))
        DEPLOY_FILES_LINUX = $$shell_quote($$shell_path($$PWD/../deploy/linux/.))
        RELEASE_DIR = Frostbite-$$RELEASE_VERSION
        RELEASE_PATH = $$shell_quote($$shell_path($$OUT_PWD/../release/$$RELEASE_DIR))
        QT_CONFIG_FILE = $$shell_quote($$shell_path($$OUT_PWD/../release/$$RELEASE_DIR/qt.conf))
        RELEASE_LIBRARIES = libQt5Widgets.so.5 libQt5Multimedia.so.5 libQt5Gui.so.5 libQt5Network.so.5 libQt5Xml.so.5 libQt5Core.so.5 \
                            libicui18n.so.66 llibicuuc.so.66 libicudata.so.66 libpng16.so.16

        # make release directory
        postbuild.commands = $$QMAKE_MKDIR $$RELEASE_PATH ;
        # copy deploy files
        postbuild.commands += $(COPY_DIR) $$DEPLOY_FILES $$RELEASE_PATH ;
        postbuild.commands += $(COPY_DIR) $$DEPLOY_FILES_LINUX $$RELEASE_PATH ;
        # copy compiled binary
        postbuild.commands += $(COPY_FILE) $$BIN $$RELEASE_PATH ;
        # qt.conf
        postbuild.commands += "echo '[Paths]\nPrefix=./\nLibraries=lib\nPlugins=plugins' > $$QT_CONFIG_FILE" ;
        # copy libraries
        postbuild.commands += $$QMAKE_MKDIR $$RELEASE_PATH/lib/ ;
        for(var, RELEASE_LIBRARIES) {
            postbuild.commands += $(COPY_FILE) -L $$[QT_INSTALL_LIBS]/$$var $$RELEASE_PATH/lib/ ;
        }
        # copy plugins
        postbuild.commands += $$QMAKE_MKDIR $$RELEASE_PATH/plugins/platforms/ ;
        postbuild.commands += $(COPY_FILE) -L $$[QT_INSTALL_PLUGINS]/platforms/libqxcb.so $$RELEASE_PATH/plugins/platforms/ ;

        postbuild.commands += $$QMAKE_MKDIR $$RELEASE_PATH/plugins/imageformats/ ;
        postbuild.commands += $(COPY_FILE) -L $$[QT_INSTALL_PLUGINS]/imageformats/*.so $$RELEASE_PATH/plugins/imageformats/ ;

        #compress package
        postbuild.commands += "tar -C $$RELEASE_PATH/../ -zcvf $$RELEASE_PATH/../frostbite-debian64.tar.gz $$RELEASE_DIR"
    }

    QMAKE_EXTRA_TARGETS += prebuild
    PRE_TARGETDEPS = prebuild
    QMAKE_EXTRA_TARGETS += postbuild
    POST_TARGETDEPS += postbuild

    QMAKE_POST_LINK += $$DEPLOY
}
