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
    qInfo() << QSqlDatabase::drivers();//当前环境支持哪些数据库
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
            qInfo() << "runtime_error:" << re.what();
        }

    }
    return dbInstance;
}

bool MusicDataBase::initDataBase()
{
    if(!m_database.open())
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                                     m_database.lastError().text());
        return false;
    }

    bool queryRes = true;
    QSqlQuery queryInit(m_database);
    //新建表:总表，历史表，我喜欢表
    queryRes &= queryInit.exec(QString("create table if not exists LocalMusic ("
                                       "hash varchar primary key,"
                                       "title varchar,"
                                       "filepath varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建音乐总表

    queryRes &= queryInit.exec(QString("create table if not exists HistoryPlayList ("
                                       "hash varchar primary key,"
                                       "title varchar,"
                                       "filepath varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                       ));//创建历史播放列表
    queryRes &= queryInit.exec(QString("create table if not exists FavorPlayList ("
                                       "hash varchar primary key,"
                                       "title varchar,"
                                       "filepath varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建我喜欢列表
    queryRes &= queryInit.exec(QString("create table if not exists ListOfPlayList (hash varchar primary key)"));//创建播放列表名称列表

    //queryRes &= createNewPlayList(tr("我喜欢"));
    qDebug()<<"创建【我喜欢】歌单："<< createNewPlayList(tr("我喜欢"));
    return queryRes;
}

int MusicDataBase::addMusicToLocalMusic(const musicDataStruct &fileData)
{
    bool queryRes = true;
    //如果是直接添加至总表
    if(fileData.hash.isEmpty() || fileData.title.isEmpty() || fileData.filepath.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁

            //查询历史列表中是否已有该歌曲，已有的话，返回添加失败
            int checkLocalRes = checkIfSongExistsInHistoryMusic(fileData.hash);

            //历史列表中已经有这首歌，重复添加了
            if(DB_OP_SUCC == checkLocalRes)
            {
                return DB_OP_ADD_REPEAT;
            }

            QSqlQuery addSongToLocal(m_database);
            QString addSongString = QString("insert into LocalMusic values('%1','%2','%3','%4','%5','%6','%7','%8')").
                    arg(fileData.hash).
                    arg(fileData.title).
                    arg(fileData.filepath).
                    arg(fileData.singer).
                    arg(fileData.album).
                    arg(fileData.filetype).
                    arg(fileData.size).
                    arg(fileData.time);
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

int MusicDataBase::delMusicFromLocalMusic(const QString& fileHash)
{
    bool queryRes = true;
    //如果是直接添加至总表
    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
            QSqlQuery checkSongFromLocal(m_database);
            QString checkSongString = QString("select * from LocalMusic where hash = '%1'").
                    arg(fileHash);
            checkSongFromLocal.exec(checkSongString);
            if(!checkSongFromLocal.next())
            {
                return INVALID_INPUT;
            }
            QSqlQuery delSongFromLocal(m_database);
            QString delSongString = QString("delete from LocalMusic where hash = '%1'").
                    arg(fileHash);
            queryRes = delSongFromLocal.exec(delSongString);

            if(queryRes)
            {
                return DB_OP_SUCC;
            }
            else
            {
                qInfo() << "数据库打开，删除失败！！！";
                return DB_OP_DEL_FAILED;
            }

        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }

    }
    //从其他歌单中也要删除
    //TODO,中间删除失败，要添加回去
    //检查歌曲是否在历史歌单中存在
    int checkHistory;
    checkHistory = checkIfSongExistsInHistoryMusic(fileHash);
    if(DB_OP_SUCC == checkHistory)
    {
        int delHistory;
        delHistory = delMusicFromHistoryMusic(fileHash);
        if(DB_OP_SUCC != delHistory)
        {
            //历史歌单中删除失败
            return delHistory;
        }
    }
    else
    {
        return checkHistory;
    }
    //检查歌曲是否在我喜欢中存在
    int checkFavor;
    checkFavor = checkIfSongExistsInFavorMusic(fileHash);
    if(DB_OP_SUCC == checkFavor)
    {
        int delFavor;
        delFavor = delMusicFromHistoryMusic(fileHash);
        if(DB_OP_SUCC != delFavor)
        {
            //历史歌单中删除失败
            return delFavor;
        }
    }
    else
    {
        return checkFavor;
    }
    //获取全部歌单
    int getList;
    QStringList ListOfplayListName;
    getList = getPlayList(ListOfplayListName);
    if(DB_OP_SUCC != getList)
    {
        return getList;
    }
    else
    {
        foreach (auto listName, ListOfplayListName)
        {
            //检查歌单列表是否存在
            int checkPlayList;
            checkPlayList = checkPlayListExist(listName);
            if(DB_OP_SUCC == checkPlayList)//歌单列表存在
            {
                //检查歌曲是否在歌单列表中存在
                int checkSong;
                checkSong = checkIfSongExistsInPlayList(fileHash, listName);
                if(DB_OP_SUCC == checkSong)//歌曲在该歌单中存在
                {
                    int delRes;
                    delRes = delMusicFromPlayList(fileHash,listName);
                    if(DB_OP_SUCC == delRes)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        return delRes;
                    }
                }
            }
            else
            {
                //此时已无法对应了
                return checkPlayList;
            }
        }
    }





}

int MusicDataBase::createNewPlayList(const QString& playListName)
{
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

//    QByteArray text = playListName.toLatin1();
//    text.toBase64();

    if(m_database.isValid())
    {
        bool createRes = true;

//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery createNewPlayList(m_database);
        QString createPlayListString = QString("create table if not exists playlist_%1 ("
                                    "hash varchar primary key,"
                                    "title varchar,"
                                    "filepath varchar,"
                                    "singer varchar,"
                                    "album varchar,"
                                    "filetype varchar,"
                                    "size varchar,"
                                    "time varchar)").arg(playListName);
        createRes &= createNewPlayList.exec(createPlayListString);

        QSqlQuery addPlayListToList(m_database);
        QString addPlayListToListString = QString("insert into ListOfPlayList values('%1')").
                arg(playListName);
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

//    QByteArray text(playListName);
//    text.toBase64();

    if(m_database.isValid())
    {
        bool delRes = true;
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery delPlayListFromList(m_database);
        QString delPlayListFromListString = QString("delete from ListOfPlayList where hash = '%1'").
                arg(playListName);
        delRes &= delPlayListFromList.exec(delPlayListFromListString);

        QSqlQuery delPlayList(m_database);
        QString delPlayListString = QString("drop table if exists playlist_%1"
                                    ).arg(playListName);
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

int MusicDataBase::getSongInfoFromLocalMusic(const QString& fileHash, musicDataStruct &fileData)
{

    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(m_database.isValid())
    {
        bool getRes = true;
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery getSongFromLocalMusic(m_database);
        QString getSongString = QString("select * from LocalMusic where hash = '%1'").arg(fileHash);
        getRes = getSongFromLocalMusic.exec(getSongString);

        if(getRes)
        {
            if(getSongFromLocalMusic.next())
            {
                fileData.hash        = getSongFromLocalMusic.value(0).toString();
                fileData.title       = getSongFromLocalMusic.value(1).toString();
                fileData.filepath    = getSongFromLocalMusic.value(2).toString();
                fileData.singer      = getSongFromLocalMusic.value(3).toString();
                fileData.album       = getSongFromLocalMusic.value(4).toString();
                fileData.filetype    = getSongFromLocalMusic.value(5).toString();
                fileData.size        = getSongFromLocalMusic.value(6).toString();
                fileData.time        = getSongFromLocalMusic.value(7).toString();
            }
            else
            {
                qInfo() << "数据库中查无此歌！";
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
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery getSongListFromLocalMusic(m_database);
        QString getSongListString = QString("select * from LocalMusic");
        getRes = getSongListFromLocalMusic.exec(getSongListString);

        if(getRes)
        {
            while(getSongListFromLocalMusic.next())
            {
                musicDataStruct temp;
                temp.hash          = getSongListFromLocalMusic.value(0).toString();
                int curRes = getSongInfoFromLocalMusic(temp.hash, temp);

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

int MusicDataBase::getSongInfoFromPlayList(musicDataStruct &fileData, const QString& songNameHash,const QString& playListName)
{

    if(songNameHash.isEmpty() || playListName.isEmpty())
    {
        return INVALID_INPUT;
    }


//    QByteArray text(playListName);
//    text.toBase64();

    if(m_database.isValid())
    {
        bool getRes = true;
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery getplayListFromLocalMusic(m_database);
        QString getplayListFromLocalMusicString = QString("select * from ListOfPlayList where hash = '%1'").arg(playListName);
        getRes = getplayListFromLocalMusic.exec(getplayListFromLocalMusicString);

        if(!getRes)
        {
            return DB_OP_GET_FAILED;
        }

        if(getplayListFromLocalMusic.next())
        {
            QSqlQuery getSongFromPlayList(m_database);
            QString getSongFromPlayListString = QString("select * from playlist_%1 where hash = '%2'").arg(playListName).arg(songNameHash);
            getRes = getSongFromPlayList.exec(getSongFromPlayListString);

            if(!getRes)
            {
                return DB_OP_GET_FAILED;
            }
            if(getSongFromPlayList.next())
            {
                fileData.hash        = getSongFromPlayList.value(0).toString();
                fileData.title       = getSongFromPlayList.value(1).toString();
                fileData.filepath    = getSongFromPlayList.value(2).toString();
                fileData.singer      = getSongFromPlayList.value(3).toString();
                fileData.album       = getSongFromPlayList.value(4).toString();
                fileData.filetype    = getSongFromPlayList.value(5).toString();
                fileData.size        = getSongFromPlayList.value(6).toString();
                fileData.time        = getSongFromPlayList.value(7).toString();
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
        return DB_UNCONNECT;
    }
}

int MusicDataBase::getSongInfoListFromPlayList(QList<musicDataStruct>& resList,const QString& playListName)
{
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }
//    QByteArray text(playListName);
//    text.toBase64();

    if(m_database.isValid())
    {
        bool getRes = true;
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        resList.clear();

        QSqlQuery getplayListFromLocalMusic(m_database);
        QString getplayListFromLocalMusicString = QString("select * from ListOfPlayList where hash = '%1'").arg(playListName);
        getRes = getplayListFromLocalMusic.exec(getplayListFromLocalMusicString);

        if(!getRes)
        {
            return DB_OP_GET_FAILED;
        }

        if(getplayListFromLocalMusic.next())
        {
            QSqlQuery getSongFromLocal(m_database);
            QString getSongFromPlayListString = QString("select * from playlist_%1").arg(playListName);
            getRes = getSongFromLocal.exec(getSongFromPlayListString);

            if(getRes)
            {
                while(getSongFromLocal.next())
                {
                    musicDataStruct temp;
                    temp.hash          = getSongFromLocal.value(0).toString();
                    int curRes = getSongInfoFromLocalMusic(temp.hash ,temp);

                    if(curRes)
                    {
                        resList.append(temp);
                    }
                    else
                    {
                        return curRes;
                    }
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
        return DB_DISORDERD;
    }
}

int MusicDataBase::addMusicToHistoryMusic(const QString& fileHash)
{
    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {

        musicDataStruct temp;
        if(m_database.isValid())
        {
            bool queryRes = true;
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
            //检查歌曲在总表中是否存在
            int checkLocalRes = getSongInfoFromLocalMusic(fileHash, temp);

            if(DB_OP_SUCC != checkLocalRes)
            {
                return checkLocalRes;
            }

            //查询历史列表中是否已有该歌曲，已有的话，返回添加失败
            int checkHistoryRes = checkIfSongExistsInHistoryMusic(fileHash);

            //历史列表中已经有这首歌，重复添加了
            if(DB_OP_SUCC == checkHistoryRes)
            {
                return DB_OP_ADD_REPEAT;
            }

            //历史列表中不存在该歌曲，添加该歌曲
            QSqlQuery addSongToHistory(m_database);
            QString addSongString = QString("insert into HistoryPlayList values('%1','%2','%3','%4','%5','%6','%7','%8')").
                    arg(temp.hash).
                    arg(temp.title).
                    arg(temp.filepath).
                    arg(temp.singer).
                    arg(temp.album).
                    arg(temp.filetype).
                    arg(temp.size).
                    arg(temp.time);
            queryRes = addSongToHistory.exec(addSongString);

            if(queryRes)
            {
                return DB_OP_SUCC;
            }
            else
            {
                qInfo() << "数据库打开，添加失败！！！";
                return DB_OP_ADD_FAILED;
            }

        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }

    }
}
int MusicDataBase::delMusicFromHistoryMusic(const QString& fileHash)
{
    bool delRes = true;

    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁

            //查询历史列表中是否已有该歌曲，已有的话，返回添加失败
            int checkHistoryRes = checkIfSongExistsInHistoryMusic(fileHash);

            if(DB_OP_SUCC == checkHistoryRes)
            {

                QSqlQuery delSongFromHistoryPlayList(m_database);

                QString delSongString = QString("delete from HistoryPlayList where hash = '%1'").
                        arg(fileHash);
                delRes &= delSongFromHistoryPlayList.exec(delSongString);

                if(delRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qInfo() << "数据库打开，删除失败！！！";
                    return DB_OP_DEL_FAILED;
                }
            }
            else
            {
                qInfo() << "数据库打开，查询失败！！！";
                return checkHistoryRes;
            }
        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }
    }
}
int MusicDataBase::getSongInfoFromHistoryMusic(const QString& fileHash, musicDataStruct &fileData)
{
    bool getRes = true;

    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
            QSqlQuery getSongInfoFromHistoryPlayList(m_database);

            QString getSongString = QString("select * from HistoryPlayList where hash = '%1'").arg(fileHash);
            getRes &= getSongInfoFromHistoryPlayList.exec(getSongString);
            if(getRes)
            {
                return DB_OP_SUCC;
            }
            else
            {
                qInfo() << "数据库打开，删除失败！！！";
                return DB_OP_GET_FAILED;
            }

            //判断是否找到了对应的歌曲
            if(getSongInfoFromHistoryPlayList.next())
            {
                fileData.hash        = getSongInfoFromHistoryPlayList.value(0).toString();
                fileData.title       = getSongInfoFromHistoryPlayList.value(1).toString();
                fileData.filepath    = getSongInfoFromHistoryPlayList.value(2).toString();
                fileData.singer      = getSongInfoFromHistoryPlayList.value(3).toString();
                fileData.album       = getSongInfoFromHistoryPlayList.value(4).toString();
                fileData.filetype    = getSongInfoFromHistoryPlayList.value(5).toString();
                fileData.size        = getSongInfoFromHistoryPlayList.value(6).toString();
                fileData.time        = getSongInfoFromHistoryPlayList.value(7).toString();

                return DB_OP_SUCC;
            }
            else
            {
                return DB_DISORDERD;
            }

        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }
    }

}

int MusicDataBase::getSongInfoListFromHistoryMusic(QList<musicDataStruct>& resList)
{


    if(m_database.isValid())
    {
        bool getRes = true;
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery getSongListFromHistoryMusic(m_database);
        QString getSongListString = QString("select * from HistoryPlayList");
        getRes = getSongListFromHistoryMusic.exec(getSongListString);

        if(getRes)
        {
            while(getSongListFromHistoryMusic.next())
            {
                musicDataStruct temp;
                temp.hash          = getSongListFromHistoryMusic.value(0).toString();
                int curRes = getSongInfoFromLocalMusic(temp.hash, temp);

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

int MusicDataBase::addMusicToFavorMusic(const QString& fileHash)
{


    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {

        musicDataStruct temp;
        if(m_database.isValid())
        {
            bool queryRes = true;
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
            //检查歌曲在总表中是否存在
            int checkLocalRes = getSongInfoFromLocalMusic(fileHash, temp);

            if(DB_OP_SUCC != checkLocalRes)
            {
                return checkLocalRes;
            }

            //查询我喜欢列表中是否已有该歌曲，已有的话，返回添加失败
            int checkFavRes = checkIfSongExistsInFavorMusic(fileHash);

            //我喜欢列表中已经有这首歌，重复添加了
            if(DB_OP_SUCC == checkFavRes)
            {
                return DB_OP_ADD_REPEAT;
            }

            //我喜欢列表中不存在该歌曲，添加该歌曲
            QSqlQuery addSongToHistory(m_database);
            QString addSongString = QString("insert into FavorPlayList values('%1','%2','%3','%4','%5','%6','%7','%8')").
                    arg(temp.hash).
                    arg(temp.title).
                    arg(temp.filepath).
                    arg(temp.singer).
                    arg(temp.album).
                    arg(temp.filetype).
                    arg(temp.size).
                    arg(temp.time);
            queryRes = addSongToHistory.exec(addSongString);

            if(queryRes)
            {
                return DB_OP_SUCC;
            }
            else
            {
                qInfo() << "数据库打开，添加失败！！！";
                return DB_OP_ADD_FAILED;
            }

        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }

    }
}

int MusicDataBase::delMusicFromFavorMusic(const QString& fileHash)
{
    bool delRes = true;

    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁

            //查询我喜欢列表中是否已有该歌曲，已有的话，返回添加失败
            int checkFavRes = checkIfSongExistsInFavorMusic(fileHash);

            if(DB_OP_SUCC == checkFavRes)
            {

                QSqlQuery delSongFromFavPlayList(m_database);

                QString delSongString = QString("delete from FavorPlayList where hash = '%1'").
                        arg(fileHash);
                delRes &= delSongFromFavPlayList.exec(delSongString);

                if(delRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qInfo() << "数据库打开，删除失败！！！";
                    return DB_OP_DEL_FAILED;
                }
            }
            else
            {
                qInfo() << "数据库打开，查询失败！！！";
                return checkFavRes;
            }
        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }
    }
}
int MusicDataBase::getSongInfoFromFavorMusic(const QString& fileHash, musicDataStruct &fileData)
{
    bool getRes = true;

    if(fileHash.isEmpty())
    {
        return INVALID_INPUT;
    }
    else
    {
        if(m_database.isValid())
        {
//            QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁


            getRes = checkIfSongExistsInFavorMusic(fileHash);

            if(getRes == DB_OP_SUCC)
            {
                QSqlQuery getSongInfoFromFavorPlayList(m_database);

                QString getSongString = QString("select * from FavorPlayList where hash = '%1'").arg(fileHash);
                getSongInfoFromFavorPlayList.exec(getSongString);
                getSongInfoFromFavorPlayList.next();

                fileData.hash        = getSongInfoFromFavorPlayList.value(0).toString();
                fileData.title       = getSongInfoFromFavorPlayList.value(1).toString();
                fileData.filepath    = getSongInfoFromFavorPlayList.value(2).toString();
                fileData.singer      = getSongInfoFromFavorPlayList.value(3).toString();
                fileData.album       = getSongInfoFromFavorPlayList.value(4).toString();
                fileData.filetype    = getSongInfoFromFavorPlayList.value(5).toString();
                fileData.size        = getSongInfoFromFavorPlayList.value(6).toString();
                fileData.time        = getSongInfoFromFavorPlayList.value(7).toString();

                return DB_OP_SUCC;
            }
            else
            {
                return DB_DISORDERD;
            }

        }
        else
        {
            qInfo() << "数据库无法打开，请重试！！！";
            return DB_UNCONNECT;
        }
    }

}
int MusicDataBase::getSongInfoListFromFavorMusic(QList<musicDataStruct>& resList)
{


    if(m_database.isValid())
    {
        bool getRes = true;
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery getSongListFromFavorMusic(m_database);
        QString getSongListString = QString("select * from FavorPlayList");
        getRes = getSongListFromFavorMusic.exec(getSongListString);

        if(getRes)
        {
            while(getSongListFromFavorMusic.next())
            {
                musicDataStruct temp;
                temp.hash          = getSongListFromFavorMusic.value(0).toString();
                int curRes = getSongInfoFromLocalMusic(temp.hash, temp);

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

int MusicDataBase::checkIfSongExistsInLocalMusic(const QString& fileHash)
{
    bool queryRes = true;

    QSqlQuery getSongFromLocalMusic(m_database);
    QString getSongString = QString("select * from LocalMusic where hash = '%1'").arg(fileHash);
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
        //外界输入hash，内部已不存在，证明数据库与外界已不一致
        return DB_DISORDERD;
    }
}

int MusicDataBase::checkIfSongExistsInHistoryMusic(const QString& fileHash)
{
    bool queryRes = true;

    QSqlQuery getSongFromHistoryMusic(m_database);
    QString getSongString = QString("select * from HistoryPlayList where hash = '%1'").arg(fileHash);
    queryRes = getSongFromHistoryMusic.exec(getSongString);

    if(false == queryRes)
    {
        return DB_OP_GET_FAILED;
    }

    //判断总表中是否存在此歌曲，不存在返回错误！
    if(getSongFromHistoryMusic.next())
    {
        return DB_OP_SUCC;
    }
    else
    {
        //外界输入hash，内部已不存在，证明数据库与外界已不一致
        return DB_DISORDERD;
    }
}

int MusicDataBase::checkIfSongExistsInFavorMusic(const QString& fileHash)
{
    bool queryRes = true;

    QSqlQuery getSongFromFavMusic(m_database);
    QString getSongString = QString("select * from FavorPlayList where hash = '%1'").arg(fileHash);
    queryRes = getSongFromFavMusic.exec(getSongString);

    if(false == queryRes)
    {
        return DB_OP_GET_FAILED;
    }

    //判断总表中是否存在此歌曲，不存在返回错误！
    if(getSongFromFavMusic.next())
    {
        return DB_OP_SUCC;
    }
    else
    {
        //外界输入hash，内部已不存在，证明数据库与外界已不一致
        return DB_DISORDERD;
    }
}

int MusicDataBase::checkIfSongExistsInPlayList(const QString& fileHash, const QString& playListName)
{
    if(fileHash.isEmpty() || playListName.isEmpty())
    {
        return INVALID_INPUT;
    }

//    QByteArray text(PlayListName);
//    text.toBase64();

    int checkRes = checkPlayListExist(playListName);

    if(DB_OP_SUCC == checkRes)
    {
        bool queryRes = true;

        QSqlQuery getSongFromPlayListMusic(m_database);
        QString getSongString = QString("select * from playlist_%1 where hash = '%2'").arg(playListName).arg(fileHash);
        queryRes = getSongFromPlayListMusic.exec(getSongString);

        if(false == queryRes)
        {
            return DB_OP_GET_FAILED;
        }

        //判断总表中是否存在此歌曲，不存在返回错误！
        if(getSongFromPlayListMusic.next())
        {
            return DB_OP_SUCC;
        }
        else
        {
            //外界输入hash，内部已不存在，证明数据库与外界已不一致
            return DB_DISORDERD;
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
            playListNameList.append(getplayListFromLocalMusic.value(0).toString());
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
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁

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
            playListNameList.append(getplayListFromLocalMusic.value(0).toString());
        }

        if(playListNameList.contains(playListName))
        {
            return DB_OP_SUCC;
        }
        else
        {
            //该列表不存在，数据库与外界已不同步
            return DB_DISORDERD;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

//添加歌曲到新建歌单，使用歌曲的HASH值,歌单名HASH值，输入数据必须有效，
int MusicDataBase::addMusicToPlayList(const QString& fileHash,const QString& playListNameHash)
{
    if(fileHash.isEmpty() || playListNameHash.isEmpty())
    {
        return INVALID_INPUT;
    }

    int checkRes;
    checkRes = checkPlayListExist(playListNameHash);

    if(DB_OP_SUCC == checkRes)
    {
        int checkLocalRes = checkIfSongExistsInLocalMusic(fileHash);

        if(DB_OP_SUCC == checkLocalRes)
        {
            musicDataStruct temp;
            if(m_database.isValid())
            {
//                QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
                //检查歌曲在总表中是否存在
                int getLocalRes = getSongInfoFromLocalMusic(fileHash, temp);

                if(DB_OP_SUCC != getLocalRes)
                {
                    return getLocalRes;
                }

                //查询歌单列表中是否已有该歌曲，已有的话，返回添加失败
                int checkPlayListRes = checkIfSongExistsInPlayList(fileHash,playListNameHash);

                //歌单列表中已经有这首歌，重复添加了
                if(DB_OP_SUCC == checkPlayListRes)
                {
                    return DB_OP_ADD_REPEAT;
                }

                int addRes;
                //歌单列表中不存在该歌曲，添加该歌曲
                QSqlQuery addSongToHistory(m_database);
                QString addSongString = QString("insert into playlist_%1 values('%2','%3','%4','%5','%6','%7','%8','%9')").
                        arg(playListNameHash).
                        arg(temp.hash).
                        arg(temp.title).
                        arg(temp.filepath).
                        arg(temp.singer).
                        arg(temp.album).
                        arg(temp.filetype).
                        arg(temp.size).
                        arg(temp.time);
                addRes = addSongToHistory.exec(addSongString);

                if(addRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qInfo() << "数据库打开，添加失败！！！";
                    return DB_OP_ADD_FAILED;
                }

            }
            else
            {
                qInfo() << "数据库无法打开，请重试！！！";
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

//从新建歌单中删除歌曲，使用歌曲的HASH值,输入数据必须有效，
int MusicDataBase::delMusicFromPlayList(const QString& fileHash,const QString& playListNameHash)
{
    if(fileHash.isEmpty() || playListNameHash.isEmpty())
    {
        return INVALID_INPUT;
    }

    int checkRes;
    checkRes = checkPlayListExist(playListNameHash);

    if(DB_OP_SUCC == checkRes)
    {
        int checkLocalRes = checkIfSongExistsInLocalMusic(fileHash);

        if(DB_OP_SUCC == checkLocalRes)
        {
            musicDataStruct temp;
            if(m_database.isValid())
            {
//                QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
                //检查歌曲在总表中是否存在
                int getLocalRes = getSongInfoFromLocalMusic(fileHash, temp);

                if(DB_OP_SUCC != getLocalRes)
                {
                    return getLocalRes;
                }

                //查询歌单列表中是否已有该歌曲，没有的话，返回删除失败
                int checkPlayListRes = checkIfSongExistsInPlayList(fileHash,playListNameHash);

                //歌单列表中已经有这首歌，可以删除
                if(DB_OP_SUCC == checkPlayListRes)
                {
                    int delRes;
                    //歌单列表中存在该歌曲，删除该歌曲
                    QSqlQuery delSongToHistory(m_database);
                    QString delSongString = QString("delete from playlist_%1 where hash = '%2'").
                            arg(playListNameHash).
                            arg(temp.hash);
                    delRes = delSongToHistory.exec(delSongString);

                    if(delRes)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        qInfo() << "数据库打开， 删除失败！！！";
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
                qInfo() << "数据库无法打开，请重试！！！";
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
