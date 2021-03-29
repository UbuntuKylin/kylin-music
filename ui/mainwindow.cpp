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

#include <QDateTime>

#include "mainwindow.h"
#include "musicDataBase.h"
#include "myapplication.h"
#include "xatom-helper.h"

#include "player.h"

MainWindow* MainWindow::mutual = nullptr; //！！！！初始化，非常重要

MainWindow::MainWindow(QString str, QWidget* parent)
    : QMainWindow(parent)
    , isFirstObject(false)
    , isPlay(false)
    , isMinimize(false)
    , isMinimode(false)
{
    qDebug() << "Mainwindow intialed at "
             << QDateTime::currentDateTime().toString(
                    "    yyyy-MM-dd hh:mm:ss.zzz"); //设置显示格式
    mutual = this; //！！！赋值，非常重要
    Single(str); //单例
    initDaemonIpcDbus(); //用户手册
    initDataBase(); //数据库
    initDbus(); //初始化dbus
    initControlse(); //初始化控件
    initGSettings(); //初始化GSettings
    initSystemTray(); //初始化托盘

    if (argName != "") {
        // kylin_music_play_request(argName);
    }
    initStyle(); //初始化样式
    qDebug() << "Mainwindow displayed at "
             << QDateTime::currentDateTime().toString(
                    "    yyyy-MM-dd hh:mm:ss.zzz"); //设置显示格式
    qDebug() << "--------------------程序初始化完成--------------------";
    isFirstObject = false; //可以接收外部命令
}

MainWindow::~MainWindow() { }

// should move to main.cpp?
void MainWindow::Single(QString path) //单例
{
    QStringList homePath
        = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString lockPath = homePath.at(0) + "/.config/kylin-music-lock";
    int fd = open(lockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR);

    if (fd < 0) {
        exit(1);
    }
    if (lockf(fd, F_TLOCK, 0)) {
        QDBusInterface interface("org.ukui.kylin_music",
            "/org/ukui/kylin_music", "org.ukui.kylin_music.play",
            QDBusConnection::sessionBus());
        QDBusReply<int> reply
            = interface.call("kylin_music_play_request", path);
        // qDebug() << "file path is " << path;
        if (reply.isValid() && reply.value() == 0) {
            // qDebug( "%d", reply.value());          // prints 4
        } else {
            qDebug() << "fail";
        }
        qDebug() << "麒麟音乐正在运行";
        exit(0);
    }
    isFirstObject = true; //我是首个对象
    argName = path;
}

void MainWindow::initStyle() //初始化样式
{
    //设置初始样式
    qDebug() << "初始化样式成功";
}

void MainWindow::initControlse() //初始化控件
{
    setAcceptDrops(true);
    qDebug() << "初始化控件成功";
}

void MainWindow::initDbus() //初始化dbus
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (sessionBus.registerService("org.ukui.kylin_music")) {
        sessionBus.registerObject(
            "/org/ukui/kylin_music", this, QDBusConnection::ExportAllContents);
        qDebug() << "初始化DBUS成功";
    } else
        qDebug() << "初始化DBUS失败";
    // S3 S4策略
    QDBusConnection::systemBus().connect(QString("org.freedesktop.login1"),
        QString("/org/freedesktop/login1"),
        QString("org.freedesktop.login1.Manager"),
        QString("PrepareForShutdown"), this, SLOT(onPrepareForShutdown(bool)));
    QDBusConnection::systemBus().connect(QString("org.freedesktop.login1"),
        QString("/org/freedesktop/login1"),
        QString("org.freedesktop.login1.Manager"), QString("PrepareForSleep"),
        this, SLOT(onPrepareForSleep(bool)));
}

void MainWindow::onPrepareForShutdown(bool Shutdown)
{
    //目前只做事件监听，不处理
    qDebug() << "onPrepareForShutdown" << Shutdown;
}

void MainWindow::onPrepareForSleep(bool isSleep)
{
    // 990
    //空指针检验
    //------此处空指针校验（如果用了指针）------
    //系统事件
    if (isSleep) {
        //如果没有正在播放则不处理
    } else {
    }
    //    if(!pauseFromPrepareForSleep)//如果休眠之前非正在播放则不处理
    //        return;
    //    pauseFromPrepareForSleep=false;

    //    play();
    //------此处继续播放------
    //注意要从记录的时间点开始播放：timeFromPrepareForSleep
    //可以模拟点击进度条的事件>
}

void MainWindow::initGSettings() //初始化GSettings
{
    if (QGSettings::isSchemaInstalled(FITTHEMEWINDOW)) {
        themeData = new QGSettings(FITTHEMEWINDOW);
        if (themeData->get("style-name").toString() == "ukui-dark"
            || themeData->get("style-name").toString() == "ukui-black") {

        } else {
        }
    }
    qDebug() << "初始化GSettings成功";
}

void MainWindow::initSystemTray() //初始化托盘
{
    qDebug() << "初始化托盘成功";
}

void MainWindow::initDaemonIpcDbus() //用户手册
{
    mDaemonIpcDbus = new DaemonIpcDbus();
    qDebug() << "包含用户手册成功";
}

void MainWindow::initDataBase() //数据库
{
    int res;
    res = g_db->initDataBase();
    if (res != DB_OP_SUCC) {
        qDebug() << "数据库加载失败" << __FILE__ << "," << __FUNCTION__ << ","
                 << __LINE__;
    }
    qDebug() << "数据库加载成功";
}

int MainWindow::kylinMusicPlayRequest(QString path)
{
    // 990
    if (isFirstObject
        && !QFileInfo::exists(path)) //首个实例不接受文件以外的参数
    {
        qDebug() << "首个实例不接受文件以外的参数";
        isFirstObject = false; //可以接收其他命令
        return 0;
    }
    if (path == "") {
        //此处显示相关代码代码无效，暂时作为已知BUG处理，后续尝试kwin接口唤醒
        if (this->isMinimized())
            this->showNormal();
        this->raise();
        this->activateWindow();
        qDebug() << "窗口置顶";
        return 0;
    }
    if (path == "-n" || path == "-next") {
        //------下一首-------
        qDebug() << "下一首";
        return 0;
    }
    if (path == "-b" || path == "-back") {
        //-------上一首------
        qDebug() << "上一首";
        return 0;
    }
    if (path == "-p" || path == "-pause") {

        //------暂停------
        qDebug() << "暂停";
        return 0;
    }
    if (path == "-s" || path == "-start") {
        //------播放------
        qDebug() << "播放";
        return 0;
    }
    QStringList qStringListPath;
    qStringListPath << path;
    processArgs(qStringListPath);
    return 0;
}

// 实现键盘响应
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!mDaemonIpcDbus->daemonIsNotRunning()) {
            // F1快捷键打开用户手册，如kylin-music
            //由于是小工具类，下面的showGuide参数要填写"tools/kylin-recorder"
            mDaemonIpcDbus->showGuide("kylin-music");
        }
    } else if (event->key() == Qt::Key_Right) {
        return;
    } else if (event->key() == Qt::Key_Left) {
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
}

#include <QPropertyAnimation>
void MainWindow::slot_showMiniWidget()
{
    // 添加过渡动画
    QPropertyAnimation* animation
        = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    connect(animation, &QPropertyAnimation::valueChanged,
        [&](QVariant value) { update(); });
    connect(animation, &QPropertyAnimation::finished, [&]() {
        hide();
        setWindowOpacity(1);
    });

    // QPropertyAnimation* animation_mini
    //     = new QPropertyAnimation(m_MiniWidget, "windowOpacity");
    // animation_mini->setDuration(200);
    // animation_mini->setStartValue(0);
    // animation_mini->setEndValue(1);
    // connect(animation_mini, &QPropertyAnimation::valueChanged,
    //     [&](QVariant value) { m_MiniWidget->update(); });

    // m_MiniWidget->setWindowOpacity(0);
    // m_MiniWidget->showNormal();
    // m_MiniWidget->activateWindow();
    // animation->start(QAbstractAnimation::DeleteWhenStopped);
    // animation_mini->start(QAbstractAnimation::DeleteWhenStopped);
    // 添加过渡动画 =======================

    //    m_MiniWidget->showNormal();
}

void MainWindow::slot_showMaximized()
{
    if (isMinimize) {
        isMinimize = false;
    } else {
        showFullScreen();
    }
}

void MainWindow::slot_showMinimized()
{
    showMinimized();
    showNormal();
}

void MainWindow::slot_closeMainWid() { close(); }

void MainWindow::slot_closeMiniWidget() { close(); }

void MainWindow::slot_recoverNormalWidget() { showNormal(); }

void MainWindow::promptMessage() { }

bool MainWindow::eventFilter(QObject* obj, QEvent* event) //鼠标滑块点击
{
    return QObject::eventFilter(obj, event);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    //    sliderWid->vSlider->hide();
    QMainWindow::mousePressEvent(event);
}

// should move to titlebar
/*
void MainWindow::showAboutWidget()
{
    qDebug() << "关于";
    // aboutWidget = new AllPupWindow(this);
    // aboutWidget->resize(420, 300);
    // aboutWidget->cancelBtn->hide();
    // //    aboutWidget->hide();
    // aboutWidget->pupDialog->show();
}
void MainWindow::menuModuleSetThemeStyle(QString str)
{
    // if ("light-theme" == str) {
    //     changeItemColour();
    //     //        changeLightTheme();
    // } else if ("dark-theme" == str) {
    //     changeItemColour();
    //     //        changeDarkTheme();
    // } else
    //     qDebug() << "切换主题样式功能异常:" << str;
}


void MainWindow::showSearchResultWidget()
{
    myTitleBar->searchWidget->hide();
    myTitleBar->searchResultWidget->musicInfoWidget->clear();
    QString enterStr = myTitleBar->searchEdit->text().trimmed();
    if (enterStr != "") {
        QSqlQuery searchQuery;
        QString searchStr
            = QString("select * from LocalMusic where musicname LIKE '%%1%'")
                  .arg(enterStr);
        searchQuery.exec(searchStr);
        while (searchQuery.next()) {
            QString songName = searchQuery.value(1).toString();
            QString Path = searchQuery.value(2).toString();
            QString Title = searchQuery.value(3).toString();
            QString Album = searchQuery.value(4).toString();
            QString Time = searchQuery.value(7).toString();

            QListWidgetItem* resultitem = new QListWidgetItem(
                myTitleBar->searchResultWidget->musicInfoWidget);
            SongItem* songitem1 = new SongItem;
            myTitleBar->searchResultWidget->musicInfoWidget->setItemWidget(
                resultitem, songitem1);

            //            songitem1->songNameLabel->setText(songName);
            //            //歌曲名称 songitem1->albumLabel->setText(Album);
            //            //专辑 songitem1->songTimeLabel->setText(Time); //时长
            //            songitem1->singerLabel->setText(Title); //歌手

            songitem1->song_singer_albumText(songName, Title, Album);
            songitem1->songTimeLabel->setText(Time); //时长

            myTitleBar->searchResultWidget->PlayList->addMedia(
                QUrl::fromLocalFile(Path));

            //        searchResultWidget->musicInfoWidget->show();
        }
        myTitleBar->searchResultWidget->songNumberLabel->setText(
            tr("A total of")
            + QString::number(
                myPlaySongArea->mybeforeList->beforePlayList->count())
            + tr("The first"));

        myTitleBar->searchResultWidget->show();
        myTitleBar->searchResultWidget->raise();
        rightlayout->replaceWidget(
            mySideBar->rightChangeWid, myTitleBar->searchResultWidget);
        mySideBar->rightChangeWid->hide();
    }
}

void MainWindow::hideSearchResultWidget()
{
    if (!(myTitleBar->searchResultWidget->isHidden())) {
        myTitleBar->searchResultWidget->hide();
        rightlayout->replaceWidget(
            myTitleBar->searchResultWidget, mySideBar->rightChangeWid);
        mySideBar->rightChangeWid->show();
    }
}
*/

void MainWindow::dragEnterEvent(QDragEnterEvent* event) //拖进事件
{
    // 判断拖拽文件类型，文件名 接收该动作
    if (event->mimeData()->hasFormat("text/uri-list")) {
        //        event->setDropAction(Qt::CopyAction);
        event->acceptProposedAction();
    }
    QWidget::dragEnterEvent(event);
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    //    event->setDropAction(Qt::MoveAction);
    QMainWindow::dragMoveEvent(event);
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    QMainWindow::dragLeaveEvent(event);
}

void MainWindow::dropEvent(QDropEvent* event) //放下事件
{
    auto urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }
    QStringList localpath;
    for (auto& url : urls) {
        localpath << url.toLocalFile();
    }
}

void MainWindow::processArgs(QStringList args) { }
