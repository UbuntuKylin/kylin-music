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

#ifndef SONGITEM_H
#define SONGITEM_H


#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <songitem.h>
#include <widgetstyle.h>
#include <QMouseEvent>
#include <QEvent>

class SongItem : public QWidget
{
    Q_OBJECT

public:

    explicit SongItem(QWidget *parent = nullptr);
    void initItem();
    QWidget *songNameWid;
    QLabel *singerLabel;
    QLabel *albumLabel;
    QLabel *songTimeLabel;
    QLabel *songNameLabel;
    QPushButton *hoverLike;
    QPushButton *hoverPlay;

    void itemcolor(int type);

    void song_singer_albumText(QString songName,QString singer,QString album); //播放列表label

    enum TypeitemType {
        defaultType = 0,
        highlightType = 1
    } itemType;

public slots:


private:

};


class HistoryListItem : public QWidget
{
    Q_OBJECT

public:

    explicit HistoryListItem(QWidget *parent = nullptr);
    void initItem();
    QWidget *songNameWid;
    QLabel *singerLabel;
    QLabel *songTimeLabel;
    QLabel *songNameLabel;

    void itemcolor();
    void song_singerText(QString songName,QString singer);  //历史记录label


public slots:


private:



};

#endif // SONGITEM_H
