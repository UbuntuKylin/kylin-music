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
    db.setDatabaseName(dirPath + "music.db");
    if(!db.open())
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                                     db.lastError().text());
        return false;
    }
    QSqlQuery query;
//    query.exec(QString("drop table LocalMusic"));
//    query.exec(QString("drop table FavoriteMusic"));
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
    query.exec(QString("create table if not exists FavoriteMusic ("
                       "id int primary key,"
                       "musicname varchar,"
                       "filepath varchar,"
                       "singer varchar,"
                       "album varchar,"
                       "type varchar,"
                       "size varchar,"
                       "time varchar)"
                       ));//播放列表数据
    return true;
}

#endif // CONNECTION_H
