QT       += core gui sql
QT       += multimedia multimediawidgets
QT       += dbus x11extras KWindowSystem

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

schemes.files = data/org.kylin-music-data.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/

TRANSLATIONS += ./translations/kylin-music_side.ts

CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/allpupwindow.cpp \
    base/main.cpp \
    base/musicDataBase.cpp \
    base/widget.cpp \
    base/widgetstyle.cpp \
    player/musicslider.cpp \
    player/playbackmodewidget.cpp \
    player/player.cpp \
    player/playsongarea.cpp \
    player/sliderwidget.cpp \
#    player/xatom-helper.cpp \
    sidebar/mytoolbutton.cpp \
    sidebar/sidebarwidget.cpp \
    tableview/scoreinfomodel.cpp \
    tableview/tablebaseview.cpp \
    tableview/tableone.cpp \
    tableview/tableviewdelegate.cpp


HEADERS += \
    base/allpupwindow.h \
    base/musicDataBase.h \
    base/widget.h \
    base/widgetstyle.h \
    player/musicslider.h \
    player/playbackmodewidget.h \
    player/player.h \
    player/playsongarea.h \
    player/sliderwidget.h \
#    player/xatom-helper.h \
    sidebar/mytoolbutton.h \
    sidebar/sidebarwidget.h \
    tableview/scoreinfomodel.h \
    tableview/tablebaseview.h \
    tableview/tableone.h \
    tableview/tableviewdelegate.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    data/org.kylin-music-data.gschema.xml
