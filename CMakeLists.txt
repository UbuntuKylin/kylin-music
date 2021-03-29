cmake_minimum_required(VERSION 3.5)
#Set Project Name
set(project_name kylin-music)
project(${project_name} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets X11Extras Network DBus Multimedia Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets X11Extras Network DBus Multimedia Sql REQUIRED)
find_package(KF5WindowSystem REQUIRED)
#Gsettting
find_package(PkgConfig REQUIRED)
pkg_check_modules(gsettings REQUIRED IMPORTED_TARGET gsettings-qt)

#add include 
include_directories(${PROJECT_SOURCE_DIR}/playcontroller/)
include_directories(${PROJECT_SOURCE_DIR}/qtsingleapplication/)
include_directories(${PROJECT_SOURCE_DIR}/taglib/)
include_directories(${PROJECT_SOURCE_DIR}/dbcontroller/)
include_directories(${PROJECT_SOURCE_DIR}/ui/)
include_directories(${PROJECT_SOURCE_DIR}/dbus/)


file(GLOB controller_SRC 
    "playcontroller/player.cpp"
    "playcontroller/player.h"
    )

file(GLOB db_controller_SRC 
    "dbcontroller/musicDataBase.cpp"
    "dbcontroller/musicDataBase.h"
    )  

file(GLOB ui_SRC 
    "ui/*.cpp"
    "ui/*.h"
    )

file(GLOB singleapp 
    "qtsingleapplication/qtlocalpeer.cpp"
    "qtsingleapplication/qtsingleapplication.cpp"
    "qtsingleapplication/qtlocalpeer.h"
    "qtsingleapplication/qtsingleapplication.h"
    )

file(GLOB dbus_SRC 
    "dbus/daemonipcdbus.cpp"
    "dbus/daemonipcdbus.h"
    )

SET(ui_RESOURCE ${PROJECT_SOURCE_DIR}/res.qrc)

add_executable(${project_name}
    main.cpp
    ${controller_SRC}
    ${db_controller_SRC}
    ${dbus_SRC}
    ${ui_SRC}
    ${ui_RESOURCE}
    ${singleapp}
)

target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Widgets  Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::DBus Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Sql)
#窗管
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::X11Extras KF5::WindowSystem -lX11)
#Gsettting
target_link_libraries(${project_name} PkgConfig::gsettings)
#taglib
target_link_libraries(${project_name} -ltag -ltag_c)
########### install ###############
#install(TARGETS kylin-music DESTINATION /usr/bin/)
#install(FILES ${PROJECT_SOURCE_DIR}/kylin-music.desktop DESTINATION /etc/xdg/autostart/)
