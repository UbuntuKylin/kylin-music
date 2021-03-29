/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MAINWID_H
#define MAINWID_H

#define KYLINRECORDER "org.kylin-music-data.settings"
#define FITTHEMEWINDOW "org.ukui.style"

#include <QGSettings>
#include <QSettings>

#include <QMainWindow>

#include <QPropertyAnimation>
#include <QStandardPaths>
#include <QTime>
#include <QTimer>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QtDBus>

#include "daemonipcdbus.h"

#include <QString>
#include <fcntl.h>
struct MusicPath {
    QString path;
    bool ischecked;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.kylin_music.play")

public:
    MainWindow(QString str, QWidget* parent = nullptr);
    ~MainWindow();

    static MainWindow* mutual; //指针类型静态成员变量

    QGSettings* themeData = nullptr;
    QString argName;
    DaemonIpcDbus* mDaemonIpcDbus;
public slots:
    void slot_showMiniWidget();
    void slot_closeMainWid();
    void slot_closeMiniWidget();
    void slot_recoverNormalWidget();
    void slot_showMinimized();
    void slot_showMaximized();

protected:
    // Mainwindow Event handlers
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override; //鼠标滑块点击
    // 键盘响应事件MainWindow
    void keyPressEvent(QKeyEvent* event) override;
    // 用户手册功能

private:
    // functions
    void Single(QString path);
    void initStyle(); // should move to widget
    void initControlse(); //初始化控件
    void initDbus(); //初始化dbus
    void initGSettings(); //初始化GSettings
    void initSystemTray(); //初始化托盘
    void initDaemonIpcDbus(); //用户手册
    void initDataBase(); //数据库
    void onPrepareForShutdown(bool Shutdown);
    void onPrepareForSleep(bool isSleep);
    int kylinMusicPlayRequest(QString path);
    void processArgs(QStringList args);
    void promptMessage();
    void changeDarkTheme(bool isplay); //切换深色主题
    void changeLightTheme(bool isplay); //切换浅色主题
    // widgets
    // miniwindow,titlebar,sidebar,playarea,playlistarea

    // flags
    bool isFirstObject;
    bool isPlay;
    bool isMinimize;
    bool isMinimode;
};
#endif // MAINWID_H
