#include "musicDataBase.h"
#include<QSqlQuery>
#include<QMessageBox>
#include<QApplication>
#include<QSqlError>
#include<QFileInfo>
#include<QDebug>
#include<QSqlRecord>
MusicDataBase::MusicDataBase(QObject *parent) : QObject(parent)
{
    qDebug() << QSqlDatabase::drivers();//当前环境支持哪些数据库
    QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
    m_database=QSqlDatabase::addDatabase("QSQLITE");
    QString dirPath = QString(getenv("HOME")) + "/.kylin_music_ver1.0_";
//TODO
//    QFileInfo oldVersion(dirPath + "mymusic.db");
//    if(oldVersion.exists())
//    {
        //读取旧版本数据库内容，并添加至新版本数据库函数
//    }
    m_database.setDatabaseName(dirPath + "mymusic.db");
}

MusicDataBase::~MusicDataBase()
{
    qDebug() << "析构";
    if(m_databaseOpenFlag)
    {
        m_database.close();
    }
}

MusicDataBase* MusicDataBase::getInstance()
{

    static MusicDataBase *dbInstance = nullptr;
    if (dbInstance == nullptr)
    {
        try
        {
            dbInstance = new MusicDataBase;
        }
        catch (const std::runtime_error &re)
        {
            qDebug() << "runtime_error:" << re.what();
        }

    }
    return dbInstance;
}

bool MusicDataBase::initDataBase()
{
    if(!m_database.open())
    {
        m_databaseOpenFlag = true;

        QMessageBox::warning(0, QObject::tr("Database Error"),
                                     m_database.lastError().text());
        return false;
    }

    bool queryRes = true;
    QSqlQuery queryInit(m_database);
    //新建表:总表，历史表，我喜欢表
    queryRes &= queryInit.exec(QString("create table if not exists LocalMusic ("
                                       "filepath varchar primary key,"
                                       "title varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建音乐总表

    queryRes &= queryInit.exec(QString("create table if not exists HistoryPlayList ("
                                       "filepath varchar primary key,"
                                       "title varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建历史播放列表
    queryRes &= queryInit.exec(QString("create table if not exists FavorPlayList ("
                                       "filepath varchar primary key,"
                                       "title varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建我喜欢列表
    queryRes &= queryInit.exec(QString("create table if not exists ListOfPlayList (title varchar primary key)"));//创建播放列表名称列表

    queryRes &= createNewPlayList("我喜欢");
    return queryRes;
}

int MusicDataBase::addMusicToLocalMusic(const musicDataStruct &fileData)
{
    bool queryRes = true;
    //如果是直接添加至总表
    if(fileData.title.isEmpty() || fileData.filepath.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
            //查询历史列表中是否已有该歌曲，已有的话，返回添加失败
            int checkLocalRes = checkIfSongExistsInLocalMusic(fileData.filepath);

            //历史列表中已经有这首歌，重复添加了
            if(DB_OP_SUCC == checkLocalRes)
            {
                return DB_OP_ADD_REPEAT;
            }

            QSqlQuery addSongToLocal(m_database);
            QString addSongString = QString("insert into LocalMusic values('%1','%2','%3','%4','%5','%6','%7')").
                    arg(inPutStringHandle(fileData.filepath)).
                    arg(inPutStringHandle(fileData.title)).
                    arg(inPutStringHandle(fileData.singer)).
                    arg(inPutStringHandle(fileData.album)).
                    arg(inPutStringHandle(fileData.filetype)).
                    arg(inPutStringHandle(fileData.size)).
                    arg(inPutStringHandle(fileData.time));
            queryRes &= addSongToLocal.exec(addSongString);

            if(queryRes)
            {
                return DB_OP_SUCC;
            }
            else
            {
                return DB_OP_ADD_FAILED;
            }
        }
        else
        {
            return DB_UNCONNECT;
        }

    }
    //如果是先添加至新建列表
    //TODO
    //添加至新建列表
}

int MusicDataBase::delMusicFromLocalMusic(const QString& filePath)
{
    bool queryRes = true;
    //如果是直接添加至总表
    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
            int checkRes = checkIfSongExistsInLocalMusic(filePath);
            if(checkRes == DB_OP_SUCC)
            {
                QSqlQuery delSongFromLocal(m_database);
                QString delSongString = QString("delete from LocalMusic where filepath = '%1'").
                        arg(inPutStringHandle(filePath));
                queryRes = delSongFromLocal.exec(delSongString);

                if(queryRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，删除失败！！！";
                    return DB_OP_DEL_FAILED;
                }
            }
            else
            {
                return INVALID_INPUT;
            }
        }
        else
        {
            qDebug() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }

    }
    //从其他歌单中也要删除
    //TODO,中间删除失败，要添加回去
    //检查歌曲是否在历史歌单中存在
}

int MusicDataBase::createNewPlayList(const QString& playListName)
{
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(m_database.isValid())
    {
        bool createRes = true;

//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery createNewPlayList(m_database);
        QString createPlayListString = QString("create table if not exists 'playlist_%1' ("
                                               "filepath varchar primary key,"
                                               "title varchar,"
                                               "singer varchar,"
                                               "album varchar,"
                                               "filetype varchar,"
                                               "size varchar,"
                                               "time varchar)").
                arg(inPutStringHandle(playListName));
        createRes &= createNewPlayList.exec(createPlayListString);

        if(true != createRes)
        {
            return CREATE_TABLE_FAILED;
        }

        QSqlQuery addPlayListToList(m_database);
        QString addPlayListToListString = QString("insert into ListOfPlayList values('%1')").
                arg(inPutStringHandle(playListName));
        createRes &= addPlayListToList.exec(addPlayListToListString);

        if(createRes)
        {
            return DB_OP_SUCC;
        }
        else
        {
            return CREATE_TABLE_FAILED;
        }

    }
    else
    {
        return DB_UNCONNECT;
    }

}

int MusicDataBase::delPlayList(const QString& playListName)
{
    //入参检查
    if(playListName.isEmpty() || playListName == "我喜欢")
    {
        return INVALID_INPUT;
    }

    if(m_database.isValid())
    {
        bool delRes = true;
        QSqlQuery delPlayListFromList(m_database);
        QString delPlayListFromListString = QString("delete from ListOfPlayList where title = '%1'").
                arg(inPutStringHandle(playListName));
        delRes &= delPlayListFromList.exec(delPlayListFromListString);

        QSqlQuery delPlayList(m_database);
        QString delPlayListString = QString("drop table if exists 'playlist_%1'"
                                    ).arg(inPutStringHandle(playListName));
        delRes &= delPlayList.exec(delPlayListString);

        if(delRes)
        {
            return DB_OP_SUCC;
        }
        else
        {
            return DEL_TABLE_FAILED;
        }

    }
    else
    {
        return DB_UNCONNECT;
    }

}

int MusicDataBase::getSongInfoFromLocalMusic(const QString& filePath, musicDataStruct &fileData)
{

    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(m_database.isValid())
    {
        bool getRes = true;
        QSqlQuery getSongFromLocalMusic(m_database);
        QString getSongString = QString("select * from LocalMusic where filepath = '%1'").arg(inPutStringHandle(filePath));
        getRes = getSongFromLocalMusic.exec(getSongString);

        if(getRes)
        {
            if(getSongFromLocalMusic.next())
            {
                fileData.filepath    = outPutStringHandle(getSongFromLocalMusic.value(0).toString());
                fileData.title       = outPutStringHandle(getSongFromLocalMusic.value(1).toString());
                fileData.singer      = outPutStringHandle(getSongFromLocalMusic.value(2).toString());
                fileData.album       = outPutStringHandle(getSongFromLocalMusic.value(3).toString());
                fileData.filetype    = outPutStringHandle(getSongFromLocalMusic.value(4).toString());
                fileData.size        = outPutStringHandle(getSongFromLocalMusic.value(5).toString());
                fileData.time        = outPutStringHandle(getSongFromLocalMusic.value(6).toString());
            }
            else
            {
                qDebug() << "数据库中查无此歌！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return DB_DISORDERD;
            }
            return DB_OP_SUCC;
        }
        else
        {
            return DB_OP_GET_FAILED;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

int MusicDataBase::getSongInfoListFromLocalMusic(QList<musicDataStruct>& resList)
{

    if(m_database.isValid())
    {
        bool getRes = true;

        QSqlQuery getSongListFromLocalMusic(m_database);
        QString getSongListString = QString("select * from LocalMusic");
        getRes = getSongListFromLocalMusic.exec(getSongListString);

        if(getRes)
        {
            while(getSongListFromLocalMusic.next())
            {
                musicDataStruct temp;
                temp.filepath = outPutStringHandle(getSongListFromLocalMusic.value(0).toString());
                int curRes = getSongInfoFromLocalMusic(temp.filepath, temp);

                if(curRes)
                {
                    resList.append(temp);
                }
                else
                {
                    return curRes;
                }
            }

            return DB_OP_SUCC;
        }
        else
        {
            return DB_OP_GET_FAILED;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

int MusicDataBase::getSongInfoFromPlayList(musicDataStruct &fileData, const QString& filePath,const QString& playListName)
{

    if(filePath.isEmpty() || playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(m_database.isValid())
    {
        int checkRes = checkPlayListExist(playListName);
        if(checkRes == DB_OP_SUCC)
        {
            bool getRes = true;
            QSqlQuery getplayList(m_database);
            QString getplayListString = QString("select * from ListOfPlayList where title = '%1'").
                    arg(inPutStringHandle(playListName));
            getRes = getplayList.exec(getplayListString);

            if(!getRes)
            {
                return DB_OP_GET_FAILED;
            }

            if(getplayList.next())
            {
                QSqlQuery getSongFromPlayList(m_database);
                QString getSongFromPlayListString = QString("select * from 'playlist_%1' where filepath = '%2'").
                        arg(inPutStringHandle(playListName)).arg(inPutStringHandle(filePath));
                getRes = getSongFromPlayList.exec(getSongFromPlayListString);

                if(!getRes)
                {
                    return DB_OP_GET_FAILED;
                }
                if(getSongFromPlayList.next())
                {
                    fileData.filepath    = outPutStringHandle(getSongFromPlayList.value(0).toString());
                    fileData.title       = outPutStringHandle(getSongFromPlayList.value(1).toString());
                    fileData.singer      = outPutStringHandle(getSongFromPlayList.value(2).toString());
                    fileData.album       = outPutStringHandle(getSongFromPlayList.value(3).toString());
                    fileData.filetype    = outPutStringHandle(getSongFromPlayList.value(4).toString());
                    fileData.size        = outPutStringHandle(getSongFromPlayList.value(5).toString());
                    fileData.time        = outPutStringHandle(getSongFromPlayList.value(6).toString());
                }
                else
                {
                    return DB_OP_GET_FAILED;
                }
                return DB_OP_SUCC;
            }
            else
            {
                return PLAYLIST_UNFOUND;
            }
        }
        else
        {
            return checkRes;
        }

    }
    else
    {
        return DB_UNCONNECT;
    }
}

int MusicDataBase::getSongInfoListFromPlayList(QList<musicDataStruct>& resList,const QString& playListName)
{
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(m_database.isValid())
    {
        int checkRes = checkPlayListExist(playListName);
        if(checkRes == DB_OP_SUCC)
        {
            bool getRes = true;
            resList.clear();

            QSqlQuery getplayList(m_database);
            QString getplayListString = QString("select * from ListOfPlayList where title = '%1'").
                    arg(inPutStringHandle(playListName));
            getRes = getplayList.exec(getplayListString);

            if(!getRes)
            {
                return DB_OP_GET_FAILED;
            }

            if(getplayList.next())
            {
                QSqlQuery getSongFromPlayList(m_database);
                QString getSongFromPlayListString = QString("select * from 'playlist_%1'").
                        arg(inPutStringHandle(playListName));
                getRes = getSongFromPlayList.exec(getSongFromPlayListString);

                if(getRes)
                {
                    while(getSongFromPlayList.next())
                    {
                        musicDataStruct temp;
                        temp.filepath      = outPutStringHandle(getSongFromPlayList.value(0).toString());
                        temp.title         = outPutStringHandle(getSongFromPlayList.value(1).toString());
                        temp.singer        = outPutStringHandle(getSongFromPlayList.value(2).toString());
                        temp.album         = outPutStringHandle(getSongFromPlayList.value(3).toString());
                        temp.filetype      = outPutStringHandle(getSongFromPlayList.value(4).toString());
                        temp.size          = outPutStringHandle(getSongFromPlayList.value(5).toString());
                        temp.time          = outPutStringHandle(getSongFromPlayList.value(6).toString());

                        resList.append(temp);
                    }

                    if(0 == resList.size())
                    {
                        return DB_DISORDERD;
                    }
                    else
                    {
                        return DB_OP_SUCC;
                    }
                }
                else
                {
                    return DB_OP_GET_FAILED;
                }

            }
            else
            {
                return DB_DISORDERD;
            }
        }
        else
        {
            return checkRes;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

int MusicDataBase::addMusicToHistoryMusic(const QString& filePath)
{
    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {

        musicDataStruct temp;
        if(m_database.isValid())
        {
            bool queryRes = true;
            //检查歌曲在总表中是否存在
            int checkLocalRes = getSongInfoFromLocalMusic(filePath, temp);

            if(DB_OP_SUCC != checkLocalRes)
            {
                return checkLocalRes;
            }

            //查询历史列表中是否已有该歌曲，已有的话，返回添加失败
            int checkHistoryRes = checkIfSongExistsInHistoryMusic(filePath);

            //历史列表中已经有这首歌，重复添加了
            if(DB_OP_SUCC == checkHistoryRes)
            {
                return DB_OP_ADD_REPEAT;
            }

            //历史列表中不存在该歌曲，添加该歌曲
            QSqlQuery addSongToHistory(m_database);
            QString addSongString = QString("insert into HistoryPlayList values('%1','%2','%3','%4','%5','%6','%7')").
                    arg(inPutStringHandle(temp.filepath)).
                    arg(inPutStringHandle(temp.title)).
                    arg(inPutStringHandle(temp.singer)).
                    arg(inPutStringHandle(temp.album)).
                    arg(inPutStringHandle(temp.filetype)).
                    arg(inPutStringHandle(temp.size)).
                    arg(inPutStringHandle(temp.time));
            queryRes = addSongToHistory.exec(addSongString);

            if(queryRes)
            {
                return DB_OP_SUCC;
            }
            else
            {
                qDebug() << "数据库打开，添加失败！！！";
                return DB_OP_ADD_FAILED;
            }

        }
        else
        {
            qDebug() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }

    }
}
int MusicDataBase::delMusicFromHistoryMusic(const QString& filePath)
{
    bool delRes = true;

    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
            //查询历史列表中是否已有该歌曲，没有的话，返回删除失败
            int checkHistoryRes = checkIfSongExistsInHistoryMusic(filePath);

            if(DB_OP_SUCC == checkHistoryRes)
            {

                QSqlQuery delSongFromHistoryPlayList(m_database);

                QString delSongString = QString("delete from HistoryPlayList where filepath = '%1'").
                        arg(inPutStringHandle(filePath));
                delRes &= delSongFromHistoryPlayList.exec(delSongString);

                if(delRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，删除失败！！！";
                    return DB_OP_DEL_FAILED;
                }
            }
            else
            {
                qDebug() << "歌曲不存在！！！";
                return checkHistoryRes;
            }
        }
        else
        {
            qDebug() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }
    }
}
int MusicDataBase::getSongInfoFromHistoryMusic(const QString& filePath, musicDataStruct &fileData)
{
    bool getRes = true;

    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
            int checkRes = checkIfSongExistsInHistoryMusic(filePath);

            if(checkRes == SONG_NOT_FOUND)
            {
                return SONG_NOT_FOUND;
            }
            else
            {
                QSqlQuery getSongInfoFromHistoryPlayList(m_database);

                QString getSongString = QString("select * from HistoryPlayList where title = '%1'").arg(inPutStringHandle(filePath));
                getRes &= getSongInfoFromHistoryPlayList.exec(getSongString);
                if(getRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，删除失败！！！";
                    return DB_OP_GET_FAILED;
                }

                //判断是否找到了对应的歌曲
                if(getSongInfoFromHistoryPlayList.next())
                {
                    fileData.filepath    = outPutStringHandle(getSongInfoFromHistoryPlayList.value(0).toString());
                    fileData.title       = outPutStringHandle(getSongInfoFromHistoryPlayList.value(1).toString());
                    fileData.singer      = outPutStringHandle(getSongInfoFromHistoryPlayList.value(2).toString());
                    fileData.album       = outPutStringHandle(getSongInfoFromHistoryPlayList.value(3).toString());
                    fileData.filetype    = outPutStringHandle(getSongInfoFromHistoryPlayList.value(4).toString());
                    fileData.size        = outPutStringHandle(getSongInfoFromHistoryPlayList.value(5).toString());
                    fileData.time        = outPutStringHandle(getSongInfoFromHistoryPlayList.value(6).toString());

                    return DB_OP_SUCC;
                }
                else
                {
                    return DB_DISORDERD;
                }

            }

        }
        else
        {
            qDebug() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }
    }

}

int MusicDataBase::getSongInfoListFromHistoryMusic(QList<musicDataStruct>& resList)
{
    if(m_database.isValid())
    {
        bool getRes = true;
        QSqlQuery getSongListFromHistoryMusic(m_database);
        QString getSongListString = QString("select * from HistoryPlayList");
        getRes = getSongListFromHistoryMusic.exec(getSongListString);

        if(getRes)
        {
            while(getSongListFromHistoryMusic.next())
            {
                musicDataStruct temp;
                temp.filepath = outPutStringHandle(getSongListFromHistoryMusic.value(0).toString());
                int curRes = getSongInfoFromLocalMusic(temp.filepath, temp);

                if(curRes)
                {
                    resList.append(temp);
                }
                else
                {
                    return curRes;
                }
            }

            return DB_OP_SUCC;
        }
        else
        {
            return DB_OP_GET_FAILED;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

int MusicDataBase::addMusicToFavorMusic(const QString& filePath)
{
//废弃
    qDebug() << "函数已废弃！！！";
    return 0;
}

int MusicDataBase::delMusicFromFavorMusic(const QString& filePath)
{
    //废弃
    qDebug() << "函数已废弃！！！";
    return 0;
}
int MusicDataBase::getSongInfoFromFavorMusic(const QString& filePath, musicDataStruct &fileData)
{
    //废弃
    qDebug() << "函数已废弃！！！";
    return 0;
}
int MusicDataBase::getSongInfoListFromFavorMusic(QList<musicDataStruct>& resList)
{
    //废弃
    qDebug() << "函数已废弃！！！";
    return 0;
}

int MusicDataBase::checkIfSongExistsInLocalMusic(const QString& filePath)
{
    bool queryRes = true;

    QString filePathHash = inPutStringHandle(filePath);

    QSqlQuery getSongFromLocalMusic(m_database);
    QString getSongString = QString("select * from LocalMusic where filepath = '%1'").arg(filePathHash);
    queryRes = getSongFromLocalMusic.exec(getSongString);

    if(false == queryRes)
    {
        return DB_OP_GET_FAILED;
    }

    //判断总表中是否存在此歌曲，不存在返回错误！
    if(getSongFromLocalMusic.next())
    {
        return DB_OP_SUCC;
    }
    else
    {
        qDebug() << "歌曲未找到";
        return SONG_NOT_FOUND;
    }
}

int MusicDataBase::checkIfSongExistsInHistoryMusic(const QString& filePath)
{
    bool queryRes = true;

    QString filePathHash = inPutStringHandle(filePath);

    QSqlQuery getSongFromHistoryMusic(m_database);
    QString getSongString = QString("select * from HistoryPlayList where filepath = '%1'").arg(filePathHash);
    queryRes = getSongFromHistoryMusic.exec(getSongString);

    if(false == queryRes)
    {
        return DB_OP_GET_FAILED;
    }

    //判断历史歌单中是否存在此歌曲，不存在返回错误！
    if(getSongFromHistoryMusic.next())
    {
        return DB_OP_SUCC;
    }
    else
    {
        //歌曲未找到
        return SONG_NOT_FOUND;
    }
}

int MusicDataBase::checkIfSongExistsInFavorMusic(const QString& filePath)
{
    //废弃
    qDebug() << "函数已废弃！！！";
    return 0;
}

int MusicDataBase::checkIfSongExistsInPlayList(const QString& filePath, const QString& playListName)
{
    if(filePath.isEmpty() || playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    QString filePathHash = inPutStringHandle(filePath);
    QString playListNameHash = inPutStringHandle(playListName);

    int checkRes = checkPlayListExist(playListName);

    if(DB_OP_SUCC == checkRes)
    {
        bool queryRes = true;

        QSqlQuery getSongFromPlayListMusic(m_database);
        QString getSongString = QString("select * from 'playlist_%1' where filepath = '%2'").arg(playListNameHash).arg(filePathHash);
        queryRes = getSongFromPlayListMusic.exec(getSongString);

        if(false == queryRes)
        {
            return DB_OP_GET_FAILED;
        }

        //判断歌曲列表中是否存在此歌曲，不存在返回错误！
        if(getSongFromPlayListMusic.next())
        {
            return DB_OP_SUCC;
        }
        else
        {
            //歌曲不存在
            return SONG_NOT_FOUND;
        }
    }
    else
    {
        return checkRes;
    }

}

int MusicDataBase::getPlayList(QStringList& playListNameList)
{
    bool getRes = true;

    if(m_database.isValid())
    {
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁

        QSqlQuery getplayListFromLocalMusic(m_database);
        QString getplayListFromLocalMusicString = QString("select * from ListOfPlayList");
        getRes = getplayListFromLocalMusic.exec(getplayListFromLocalMusicString);

        if(!getRes)
        {
            return DB_OP_GET_FAILED;
        }

        while(getplayListFromLocalMusic.next())
        {
            playListNameList.append(outPutStringHandle(getplayListFromLocalMusic.value(0).toString()));
        }

        return DB_OP_SUCC;
    }
    else
    {
        return DB_DISORDERD;
    }
}

int MusicDataBase::checkPlayListExist(const QString& playListName)
{
    bool getRes = true;

    QStringList playListNameList;

    if(m_database.isValid())
    {
        QSqlQuery getplayListFromLocalMusic(m_database);
        QString getplayListFromLocalMusicString = QString("select * from ListOfPlayList");
        getRes = getplayListFromLocalMusic.exec(getplayListFromLocalMusicString);

        if(!getRes)
        {
            return DB_OP_GET_FAILED;
        }

        playListNameList.clear();
        while(getplayListFromLocalMusic.next())
        {
            playListNameList.append(outPutStringHandle(getplayListFromLocalMusic.value(0).toString()));
        }

        if(playListNameList.contains(playListName))
        {
            return DB_OP_SUCC;
        }
        else
        {
            //该列表不存在，数据库与外界已不同步
            return LIST_NOT_FOUND;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

//添加歌曲到新建歌单，使用歌曲的歌名值,歌单名值，输入数据必须有效，
int MusicDataBase::addMusicToPlayList(const QString& filePath,const QString& playListName)
{
    if(filePath.isEmpty() || playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    int checkRes;
    checkRes = checkPlayListExist(playListName);

    if(DB_OP_SUCC == checkRes)
    {
        int checkLocalRes = checkIfSongExistsInLocalMusic(filePath);

        if(DB_OP_SUCC == checkLocalRes)
        {
            musicDataStruct temp;
            if(m_database.isValid())
            {
                //从总表中添加的，故要从总表中取歌曲信息！！！
                //TODO缺少直接添加文件到播放列表的接口
                int getLocalRes = getSongInfoFromLocalMusic(filePath, temp);

                if(DB_OP_SUCC != getLocalRes)
                {
                    return getLocalRes;
                }
                //查询歌单列表中是否已有该歌曲，已有的话，返回添加失败
                int checkPlayListRes = checkIfSongExistsInPlayList(temp.filepath,playListName);

                //歌单列表中已经有这首歌，重复添加了
                if(DB_OP_SUCC == checkPlayListRes)
                {
                    return DB_OP_ADD_REPEAT;
                }

                int addRes;
                //歌单列表中不存在该歌曲，添加该歌曲
                QSqlQuery addSongToHistory(m_database);
                QString addSongString = QString("insert into 'playlist_%1' values('%2','%3','%4','%5','%6','%7','%8')").
                        arg(inPutStringHandle(playListName)).
                        arg(inPutStringHandle(temp.filepath)).
                        arg(inPutStringHandle(temp.title)).
                        arg(inPutStringHandle(temp.singer)).
                        arg(inPutStringHandle(temp.album)).
                        arg(inPutStringHandle(temp.filetype)).
                        arg(inPutStringHandle(temp.size)).
                        arg(inPutStringHandle(temp.time));
                addRes = addSongToHistory.exec(addSongString);

                if(addRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，添加失败！！！";
                    return DB_OP_ADD_FAILED;
                }

            }
            else
            {
                qDebug() << "数据库无法打开，请重试！！！";
                return DB_UNCONNECT;
            }

        }
        else
        {
            //本地歌单中检查该歌曲失败
            return checkLocalRes;
        }
    }
    else
    {
        //该歌单检查是否存在失败
        return checkRes;
    }

}

//从新建歌单中删除歌曲，使用歌曲的路径值,输入数据必须有效，
int MusicDataBase::delMusicFromPlayList(const QString& filePath,const QString& playListName)
{
    if(filePath.isEmpty() || playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    int checkRes;
    checkRes = checkPlayListExist(playListName);

    if(DB_OP_SUCC == checkRes)
    {
        //暂时把歌单和本地歌曲分离
//        int checkLocalRes = checkIfSongExistsInLocalMusic(filePath);

//        if(DB_OP_SUCC == checkLocalRes)
//        {
            if(m_database.isValid())
            {
                //查询歌单列表中是否已有该歌曲，没有的话，返回删除失败
                int checkPlayListRes = checkIfSongExistsInPlayList(filePath,playListName);

                //歌单列表中已经有这首歌，可以删除
                if(DB_OP_SUCC == checkPlayListRes)
                {
                    int delRes;
                    //歌单列表中存在该歌曲，删除该歌曲
                    QSqlQuery delSongToHistory(m_database);
                    QString delSongString = QString("delete from 'playlist_%1' where filepath = '%2'").
                            arg(inPutStringHandle(playListName)).
                            arg(inPutStringHandle(filePath));
                    delRes = delSongToHistory.exec(delSongString);

                    if(delRes)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        qDebug() << "数据库打开， 删除失败！！！";
                        return DB_OP_DEL_FAILED;
                    }
                }
                else
                {
                    return DB_OP_DEL_FAILED;
                }


            }
            else
            {
                qDebug() << "数据库无法打开，请重试！！！";
                return DB_UNCONNECT;
            }

//        }
//        else
//        {
//            //本地歌单中检查该歌曲失败
//            return checkLocalRes;
//        }
    }
    else
    {
        //该歌单检查是否存在失败
        return checkRes;
    }
}


QString MusicDataBase::inPutStringHandle(const QString& input)
{
    QString temp = input;
    QByteArray text = temp.toUtf8();

    QString output = text.toBase64();
    return output;
}

QString MusicDataBase::outPutStringHandle(const QString& output)
{
    QString temp = output;
    QByteArray text = temp.toUtf8();
    text = QByteArray::fromBase64(text);
    QString outOrigin;
    outOrigin.prepend(text.data());
    return outOrigin;
}
