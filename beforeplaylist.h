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

#ifndef NOWPLAYLIST_H
#define NOWPLAYLIST_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QAction>
#include <QMenu>
#include <QMediaPlayer>
#include <QMediaPlaylist>


#include "songitem.h"

class BeforePlayList : public QWidget
{
    Q_OBJECT

public:

    explicit BeforePlayList(QWidget *parent = nullptr);
    void initUi();   //初始化样式
    void historyItemColor();

    QListWidget *beforePlayList;
    QLabel *beforeListTitleLabel;
    QLabel *beforeListNumberLabel;
    QToolButton *emptyBtn;

    QMediaPlaylist *PlayList;
    QMediaPlayer *Music;

    QMenu *historyMenu;
    QAction *playAction;
    QAction *nextAction;
    QAction *delAction;

    QStringList historyMusicid;

    int currentMusicPlaylist = 20;
    int currentPlayIndex = -1;

    bool isStartPlay = false;
    bool m_isMusicInitialed = false;
    void initialQMediaPlayer();
public slots:
    void on_historyWidget_customContextMenuRequested(const QPoint &pos);    //历史列表右键菜单
    void historyPlay();
    void historyNext();
    void historyDel();

private:
    void initAction(); //初始化连接

};







#endif // NOWPLAYLIST_H
