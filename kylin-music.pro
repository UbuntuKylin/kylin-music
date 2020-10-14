QT       += core gui sql widgets
QT       += multimedia multimediawidgets

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# OBJECTS_DIR = tmp
# MOC_DIR = tmp

TARGET = kylin-music

target.path = /usr/bin
target.source += $$TARGET

icon.path = /usr/share/pixmaps
icon.files = img/kylin-music.png

desktop.path = /usr/share/applications/
desktop.files = kylin-music.desktop
#appdesktop.path = /usr/share/applications/
#appdesktop.files += kylin-music.desktop

INSTALLS += target icon desktop


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changelistwid.cpp \
    main.cpp \
    mainwid.cpp \
    playsongarea.cpp \
    sidebar.cpp \
    titlebar.cpp \
    slider.cpp

HEADERS += \
    changelistwid.h \
    mainwid.h \
    playsongarea.h \
    sidebar.h \
    titlebar.h \
    connection.h \
    slider.h

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
