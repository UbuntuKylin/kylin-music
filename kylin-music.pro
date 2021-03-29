QT       += core gui sql widgets
QT       += dbus x11extras KWindowSystem
QT       += multimedia multimediawidgets

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# OBJECTS_DIR = tmp
# MOC_DIR = tmp

TARGET = kylin-music

LIBS += -ltag -ltag_c

# 适配窗口管理器圆角阴影
LIBS +=-lpthread
LIBS +=-lX11

INCLUDEPATH += qtsingleapplication
DEPENDPATH += qtsingleapplication

INCLUDEPATH +=  ui/ \
                dbus/ \
                playcontroller/ \
                dbcontroller/

INCLUDEPATH += ./taglib

#!system($$PWD/translations/generate_translations_pm.sh): error("Failed to generate pm")

target.path = /usr/bin
target.source += $$TARGET

icon.path = /usr/share/pixmaps
icon.files = img/kylin-music.png

desktop.path = /usr/share/applications/
desktop.files = kylin-music.desktop

schemes.files = data/org.kylin-music-data.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/

TRANSLATIONS += ./translations/kylin-music_zh_CN.ts

INSTALLS += target \
            icon \
            desktop \
            schemes


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#main
SOURCES += main.cpp

#ui
SOURCES += \
    ui/mainwindow.cpp \
    ui/xatom-helper.cpp

HEADERS += \
    ui/mainwindow.h \
    ui/myapplication.h \
    ui/xatom-helper.h
#control
SOURCES += \
    dbus/daemonipcdbus.cpp \
    dbcontroller/musicDataBase.cpp \
    playcontroller/player.cpp

HEADERS += \
    dbus/daemonipcdbus.h \
    dbcontroller/musicDataBase.h \
    playcontroller/player.h

#singleton and others
SOURCES += \
    qtsingleapplication/qtlocalpeer.cpp \
    qtsingleapplication/qtsingleapplication.cpp

HEADERS += \
    qtsingleapplication/qtlocalpeer.h \
    qtsingleapplication/qtsingleapplication.h

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    data/org.kylin-music-data.gschema.xml
