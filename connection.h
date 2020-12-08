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

#ifndef CONNECTION_H
#define CONNECTION_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>
#include<QApplication>
#include<QSqlError>

static bool CreatConnection()
{

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    QString dirPath = QString(getenv("HOME")) + "/.kylin-music";
    db.setDatabaseName(dirPath + "mymusic.db");
    if(!db.open())
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                                     db.lastError().text());
        return false;
    }
    QSqlQuery query;
//    query.exec(QString("drop table LocalMusic"));
//    query.exec(QString("drop table NewPlayList"));
//    query.exec(QString("drop table HistoryPlayList"));
//    query.exec(QString("drop table SearchPlayList"));
    query.exec(QString("create table if not exists LocalMusic ("
                       "id int primary key,"
                       "musicname varchar,"
                       "filepath varchar,"
                       "singer varchar,"
                       "album varchar,"
                       "type varchar,"
                       "size varchar,"
                       "time varchar)"
                       ));//本地音乐列表数据
    query.exec(QObject::tr("create table if not exists NewPlayList ("
//                       "playlistid int primary key,"
                       "listname varchar primary key,"
                       "musicid varchar)"
                       ));//添加新歌单

    query.exec(QString("insert into NewPlayList (listname,musicid) values ('我喜欢','')"));

    query.exec(QString("create table if not exists HistoryPlayList ("
                       "id int primary key,"
                       "mp3Name varchar,"
                       "artistStr varchar,"
                       "timeStr varchar)"
                       ));//历史播放列表

//    query.exec(QString("create table if not exists SearchPlayList ("
//                       "id int primary key,"
//                       "musicname varchar,"
//                       "filepath varchar,"
//                       "singer varchar,"
//                       "album varchar,"
//                       "type varchar,"
//                       "size varchar,"
//                       "time varchar)"
//                       ));//搜索列表

    return true;
}

#endif // CONNECTION_H
