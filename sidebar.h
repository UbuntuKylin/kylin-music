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

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialog>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "changelistwid.h"
#include "songitem.h"
#include "allpupwindow.h"
#include "widgetstyle.h"
#include "playsongarea.h"

class SideBar :public QFrame
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

    QLabel *recommendLabel;
    QPushButton *logoLabel;
    QLabel *logoNameLabel;
    QToolButton *PlayListBtn;
    QLabel *MySongListLabel;
    QPushButton *addSongListBtn;

    QToolButton *pButton; //按钮text

    QListWidgetItem *newSongList[20]; //包括我喜欢在内的所有item

    QListWidget *songListWidget;

    QToolButton *newSongListBtn[20];   //包括我喜欢在内所有的歌单按钮

    QStackedWidget *rightChangeWid;  //进行界面切换      右部除标题栏和播放区域之外的部分

    MusicListWid *musicListChangeWid[20];  //我喜欢，以及预留出的新建歌单对应的切换界面
    MusicListWid *myMusicListWid;   //歌曲列表按钮对应的切换界面

    QSqlTableModel *sidemodel;
    QSqlTableModel *readmodel;
    QStringList allmusiclist;
    QStringList allListName;


    AllPupWindow *newSonglistPup;  //新建歌单弹出窗口

    AllPupWindow *renameSongListPup;  //重命名弹窗

    AllPupWindow *promptRenamePlayList;     //默认歌单重命名第二次弹窗

    AllPupWindow *promptSongListPup;  //  您确定删除歌单吗?

    AllPupWindow *promptRemovePlayList;  //默认歌单删除第二次弹窗

    AllPupWindow *promptExistListPup;  //歌单名已存在

    void get_listmusic_information(int listindex);

    QMenu *menu;
    QAction *listPlayAct;
    QAction *listNextAct;
    QAction *listDeleAct;
    QAction *listSongAct;

    // 当前播放列表索引
    int currentPlayList = 0;
    // 当前选中列表索引
    int currentSelectList = -1;

    int btnIndex;

    void sidecolor();

    int currentPlayIndex = -1;   //高亮相关
    // 判断删除歌单歌曲时跳过高亮
    bool skipPlaylistHighlight = false;
private:

public slots:

    void addSongList();
    void createSongList();  //新建歌单

    void ChangePage();
    void AlterPage();
//    void AlterthisPage();

    void on_musicListChangeWid_customContextMenuRequested(const QPoint &pos);
    void listPlayAct_slot();
    void listNextAct_slot();
    void deleteMusicFromSongList();    // 从歌单中删除歌曲
    void listSongAct_slot();

//    void updataplaylistwidget(int value);//更新playlistWidget
//    void on_musicListChangeWid_doubleClicked(QListWidgetItem *item);
//    void Music_playlist_stateChang(QMediaPlayer::State state);//播放状态改变


private:
    void initTopWidget();

};



#endif // SIDEBAR_H
