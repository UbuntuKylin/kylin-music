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
    QString dirPath = QString(getenv("HOME")) + "/.config/.kylin_music_ver1.1_";
//TODO
    QFileInfo oldVersion(QString(getenv("HOME")) + "/.config/.kylin_music_ver1.0_" + "mymusic.db");
    if(oldVersion.exists())
    {
        //读取旧版本数据库内容，并添加至新版本数据库函数
        qDebug() << "存在旧版本数据库" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
    }
    m_database.setDatabaseName(dirPath + "mymusic.db");
}

MusicDataBase::~MusicDataBase()
{
    qDebug() << "析构";
    if(true == m_databaseOpenFlag)
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

int MusicDataBase::initDataBase()
{
    if(!m_database.open())
    {
        m_databaseOpenFlag = true;

        QMessageBox::warning(0, QObject::tr("Database Error"),
                                     m_database.lastError().text());
        return DB_UNCONNECT;
    }

    bool queryRes = true;
    QSqlQuery queryInit(m_database);
    //新建表:总表，历史表，我喜欢表
    queryRes &= queryInit.exec(QString("create table if not exists LocalMusic ("
                                       "id integer primary key autoincrement,"
                                       "idIndex integer unique,"
                                       "filepath varchar unique not NULL,"
                                       "title varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建音乐总表，自增id为主键，index为唯一值，插入歌曲时为空，获取自增id值后赋值，filepath为唯一值且不为空。

    queryRes &= queryInit.exec(QString("create table if not exists HistoryPlayList ("
                                       "id integer primary key autoincrement,"
                                       "idIndex integer unique,"
                                       "filepath varchar unique not NULL,"
                                       "title varchar,"
                                       "singer varchar,"
                                       "album varchar,"
                                       "filetype varchar,"
                                       "size varchar,"
                                       "time varchar)"
                                       ));//创建历史播放列表，自增id为主键，index为唯一值，插入歌曲时为空，获取自增id值后赋值，filepath为唯一值且不为空。

    queryRes &= queryInit.exec(QString("create table if not exists ListOfPlayList (title varchar primary key)"));//创建播放列表名称列表


    if(true == queryRes)
    {
        qDebug()<<"本地列表，历史列表，歌单表创建成功！";

        //创建我喜欢列表
        //先检查是否存在
        int checkRes;
        checkRes = checkPlayListExist("我喜欢");
        if(LIST_NOT_FOUND == checkRes)//我喜欢列表不存在才创建
        {
            int createRes;
            createRes = createNewPlayList("我喜欢");

            return createRes;
        }
        else
        {
            return checkRes;
        }
    }
    else
    {
        qDebug() << "初始化建表失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return CREATE_TABLE_FAILED;
    }
}

int MusicDataBase::addMusicToLocalMusic(const musicDataStruct &fileData)
{
    bool queryRes = true;

    if(fileData.title.isEmpty() || fileData.filepath.isEmpty())
    {
        qDebug() << "无效入参" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }
    else
    {
        if(true == m_database.isValid())
        {
            //查询历史列表中是否已有该歌曲，已有的话，返回添加失败
            int checkLocalRes = checkIfSongExistsInLocalMusic(fileData.filepath);

            //历史列表中已经有这首歌，重复添加了
            if(DB_OP_SUCC == checkLocalRes)
            {
                qDebug() << "添加失败，重复添加" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return DB_OP_ADD_REPEAT;
            }

            QSqlQuery addSongToLocal(m_database);
            QString addSongString = QString("insert into LocalMusic (filepath,title,singer,album,filetype,size,time) values('%1','%2','%3','%4','%5','%6','%7')").
                    arg(inPutStringHandle(fileData.filepath)).
                    arg(inPutStringHandle(fileData.title)).
                    arg(inPutStringHandle(fileData.singer)).
                    arg(inPutStringHandle(fileData.album)).
                    arg(inPutStringHandle(fileData.filetype)).
                    arg(inPutStringHandle(fileData.size)).
                    arg(inPutStringHandle(fileData.time));
            queryRes &= addSongToLocal.exec(addSongString);
            //插入歌曲时自增id和idIndex无法赋值，插入后取得自增id，给idIndex赋值
            int tempIndex = addSongToLocal.lastInsertId().toInt();
            bool setRes = true;
            QSqlQuery setSongIDFromLocal(m_database);
            QString setIndex = QString("update LocalMusic set idIndex='%1' WHERE filepath='%2'").arg(tempIndex).arg(inPutStringHandle(fileData.filepath));
            setRes &= setSongIDFromLocal.exec(setIndex);

            if(true == (queryRes&setRes))
            {
                return DB_OP_SUCC;
            }
            else
            {
                qDebug() << "数据库操作失败，添加失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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

    if(filePath.isEmpty())
    {
        qDebug() << "无效入参" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }
    else
    {
        if(true == m_database.isValid())
        {
            int checkRes = checkIfSongExistsInLocalMusic(filePath);
            if(checkRes == DB_OP_SUCC)
            {
                QSqlQuery delSongFromLocal(m_database);
                QString delSongString = QString("delete from LocalMusic where filepath = '%1'").
                        arg(inPutStringHandle(filePath));
                queryRes = delSongFromLocal.exec(delSongString);

                if(true == queryRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，删除失败！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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
            qDebug() << "数据库无法打开，请重试！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
            return DB_UNCONNECT;
        }

    }
}

int MusicDataBase::createNewPlayList(const QString& playListName)
{
    if(playListName.isEmpty())
    {
        qDebug() << "无效入参" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
    {
        bool createRes = true;

//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁
        QSqlQuery createNewPlayList(m_database);
        QString createPlayListString = QString("create table if not exists 'playlist_%1' ("
                                               "id integer primary key autoincrement,"
                                               "idIndex integer unique,"
                                               "filepath varchar unique not NULL,"
                                               "title varchar,"
                                               "singer varchar,"
                                               "album varchar,"
                                               "filetype varchar,"
                                               "size varchar,"
                                               "time varchar)").
                arg(inPutStringHandle(playListName));//创建新建播放列表，自增id为主键，index为唯一值，插入歌曲时为空，获取自增id值后赋值，filepath为唯一值且不为空。
        createRes &= createNewPlayList.exec(createPlayListString);

        if(true != createRes)
        {
            return CREATE_TABLE_FAILED;
        }

        QSqlQuery addPlayListToList(m_database);
        QString addPlayListToListString = QString("insert into ListOfPlayList (title) values('%1')").
                arg(inPutStringHandle(playListName));
        createRes &= addPlayListToList.exec(addPlayListToListString);

        if(true == createRes)
        {
            return DB_OP_SUCC;
        }
        else
        {
            qDebug() << "创建新歌单失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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
        qDebug() << "无效入参" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
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

        if(true == delRes)
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
        qDebug() << "入参无效" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
    {
        bool getRes = true;
        QSqlQuery getSongFromLocalMusic(m_database);
        QString getSongString = QString("select * from LocalMusic where filepath = '%1'").arg(inPutStringHandle(filePath));
        getRes = getSongFromLocalMusic.exec(getSongString);

        if(true == getRes)
        {
            if(getSongFromLocalMusic.next())
            {
                fileData.filepath    = outPutStringHandle(getSongFromLocalMusic.value(2).toString());
                fileData.title       = outPutStringHandle(getSongFromLocalMusic.value(3).toString());
                fileData.singer      = outPutStringHandle(getSongFromLocalMusic.value(4).toString());
                fileData.album       = outPutStringHandle(getSongFromLocalMusic.value(5).toString());
                fileData.filetype    = outPutStringHandle(getSongFromLocalMusic.value(6).toString());
                fileData.size        = outPutStringHandle(getSongFromLocalMusic.value(7).toString());
                fileData.time        = outPutStringHandle(getSongFromLocalMusic.value(8).toString());
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
    if(true == m_database.isValid())
    {
        bool getRes = true;

        QSqlQuery getSongListFromLocalMusic(m_database);
        QString getSongListString = QString("select * from LocalMusic  order by idIndex");//按index排序返回给前端，而非添加歌曲时的顺序
        getRes = getSongListFromLocalMusic.exec(getSongListString);

        if(true == getRes)
        {
            while(getSongListFromLocalMusic.next())
            {
                musicDataStruct temp;
                temp.filepath = outPutStringHandle(getSongListFromLocalMusic.value(2).toString());
                int curRes = getSongInfoFromLocalMusic(temp.filepath, temp);

                if(DB_OP_SUCC == curRes)
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

int MusicDataBase::changeSongOrderInLocalMusic(const QString& selectFilePath, const QString& destinationFilePath)
{
    //入参检查
    if(selectFilePath.isEmpty() || destinationFilePath.isEmpty())
    {
        qDebug() << "入参为空" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;//入参为空
    }
    if(DB_OP_SUCC != checkIfSongExistsInLocalMusic(selectFilePath) || DB_OP_SUCC != checkIfSongExistsInLocalMusic(destinationFilePath))
    {
        qDebug() << "歌曲在歌曲总表中不存在" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;//歌曲在歌曲总表中不存在
    }
    if(selectFilePath == destinationFilePath)//位置没变化
    {
        qDebug() << "位置没变化，不用处理" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return DB_OP_SUCC;//位置没变化，不用处理
    }
    //获取歌曲的index信息
    int oldIndex = 0, newIndex = 0;
    int getRes1 = getSongIndexFromLocalMusic(selectFilePath, oldIndex);
    int getRes2 = getSongIndexFromLocalMusic(destinationFilePath, newIndex);

    if(getRes1 != DB_OP_SUCC)
    {
        qDebug() << "歌曲原INDEX获取失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return getRes1;
    }
    if(getRes2 != DB_OP_SUCC)
    {
        qDebug() << "目标歌曲INDEX获取失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return getRes2;
    }

    if(oldIndex < newIndex)//靠前的位置拖拽到了靠后的位置
    {
        bool setRes = true;
        QSqlQuery setSongIndexFromLocal(m_database);
        QString setIndex = QString("update LocalMusic set idIndex=0 WHERE filepath='%1'").arg(inPutStringHandle(selectFilePath));
        setRes &= setSongIndexFromLocal.exec(setIndex);

        if(true == setRes)
        {
            bool getRes = true;
            QSqlQuery getBetweenSongIndexFromLocal(m_database);
            QString setIndexs = QString("select * from LocalMusic WHERE idIndex between '%1' and '%2'").arg(oldIndex).arg(newIndex);
            getRes &= getBetweenSongIndexFromLocal.exec(setIndexs);

            if(true == getRes)
            {
                bool updateIndexRes = true;
                while(getBetweenSongIndexFromLocal.next())
                {
                    int tempIndex           = getBetweenSongIndexFromLocal.value(1).toInt();
                    QString tempFilepath    = outPutStringHandle(getBetweenSongIndexFromLocal.value(2).toString());

                    QSqlQuery updateSongIndexFromLocal(m_database);
                    QString updateIndex = QString("update LocalMusic set idIndex='%1' WHERE filepath='%2'").arg(tempIndex-1).arg(inPutStringHandle(tempFilepath));
                    updateIndexRes &= updateSongIndexFromLocal.exec(updateIndex);
                }

                if(true == updateIndexRes)
                {
                    bool setRes2 = true;
                    QSqlQuery setSongIndexFromLocal2(m_database);
                    QString setIndex2 = QString("update LocalMusic set idIndex='%1' WHERE filepath='%2'").arg(newIndex).arg(inPutStringHandle(selectFilePath));
                    setRes2 &= setSongIndexFromLocal2.exec(setIndex2);

                    if(true == setRes2)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        return LIST_REORDER_ERR;
                    }
                }
                else
                {
                    return LIST_REORDER_ERR;
                }
            }
        }
    }
    else if(oldIndex > newIndex)//靠后的位置拖拽到了靠前的位置
    {
        bool setRes = true;
        QSqlQuery setSongIndexFromLocal(m_database);
        QString setIndex = QString("update LocalMusic set idIndex=0 WHERE filepath='%1'").arg(inPutStringHandle(selectFilePath));
        setRes &= setSongIndexFromLocal.exec(setIndex);

        if(true == setRes)
        {
            bool getRes = true;
            QSqlQuery getBetweenSongIndexFromLocal(m_database);
            QString setIndexs = QString("select * from LocalMusic WHERE idIndex between '%1' and '%2' order by idIndex desc").arg(newIndex+1).arg(oldIndex);
            getRes &= getBetweenSongIndexFromLocal.exec(setIndexs);

            if(true == getRes)
            {
                bool updateIndexRes = true;
                while(getBetweenSongIndexFromLocal.next())
                {
                    int tempIndex           = getBetweenSongIndexFromLocal.value(1).toInt();
                    QString tempFilepath    = outPutStringHandle(getBetweenSongIndexFromLocal.value(2).toString());

                    QSqlQuery updateSongIndexFromLocal(m_database);
                    QString updateIndex = QString("update LocalMusic set idIndex='%1' WHERE filepath='%2'").arg(tempIndex+1).arg(inPutStringHandle(tempFilepath));
                    updateIndexRes &= updateSongIndexFromLocal.exec(updateIndex);
                }

                if(true == updateIndexRes)
                {
                    bool setRes2 = true;
                    QSqlQuery setSongIndexFromLocal2(m_database);
                    QString setIndex2 = QString("update LocalMusic set idIndex='%1' WHERE filepath='%2'").arg(newIndex+1).arg(inPutStringHandle(selectFilePath));
                    setRes2 &= setSongIndexFromLocal2.exec(setIndex2);

                    if(true == setRes2)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        return LIST_REORDER_ERR;
                    }
                }
                else
                {
                    return LIST_REORDER_ERR;
                }
            }
        }
    }
    else//位置没变
    {
        return DB_OP_SUCC;
    }
}

int MusicDataBase::getSongInfoFromPlayList(musicDataStruct &fileData, const QString& filePath,const QString& playListName)
{

    if(filePath.isEmpty() || playListName.isEmpty())
    {
        qDebug() << "无效入参" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
    {
        int checkRes = checkPlayListExist(playListName);
        if(checkRes == DB_OP_SUCC)
        {
            bool getRes = true;
            QSqlQuery getplayList(m_database);
            QString getplayListString = QString("select * from ListOfPlayList where title = '%1'").
                    arg(inPutStringHandle(playListName));
            getRes = getplayList.exec(getplayListString);

            if(false == getRes)
            {
                qDebug() << "歌单表中查询歌单失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return DB_OP_GET_FAILED;
            }

            if(getplayList.next())
            {
                QSqlQuery getSongFromPlayList(m_database);
                QString getSongFromPlayListString = QString("select * from 'playlist_%1' where filepath = '%2'").
                        arg(inPutStringHandle(playListName)).arg(inPutStringHandle(filePath));
                getRes = getSongFromPlayList.exec(getSongFromPlayListString);

                if(false == getRes)
                {
                    return DB_OP_GET_FAILED;
                }
                if(getSongFromPlayList.next())
                {
                    fileData.filepath    = outPutStringHandle(getSongFromPlayList.value(2).toString());
                    fileData.title       = outPutStringHandle(getSongFromPlayList.value(3).toString());
                    fileData.singer      = outPutStringHandle(getSongFromPlayList.value(4).toString());
                    fileData.album       = outPutStringHandle(getSongFromPlayList.value(5).toString());
                    fileData.filetype    = outPutStringHandle(getSongFromPlayList.value(6).toString());
                    fileData.size        = outPutStringHandle(getSongFromPlayList.value(7).toString());
                    fileData.time        = outPutStringHandle(getSongFromPlayList.value(8).toString());
                }
                else
                {
                    qDebug() << "歌单中未查询到歌曲" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                    return DB_OP_GET_FAILED;
                }
                return DB_OP_SUCC;
            }
            else
            {
                qDebug() << "歌单表中未查询到该歌单" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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
        qDebug() << "输入歌单名为空" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
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

            if(false == getRes)
            {
                qDebug() << "歌单表中查询歌单失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return DB_OP_GET_FAILED;
            }

            if(getplayList.next())
            {
                QSqlQuery getSongFromPlayList(m_database);
                QString getSongFromPlayListString = QString("select * from 'playlist_%1' order by idIndex").
                        arg(inPutStringHandle(playListName));//按index排序返回给前端，而非添加歌曲时的顺序
                getRes = getSongFromPlayList.exec(getSongFromPlayListString);

                if(true == getRes)
                {
                    while(getSongFromPlayList.next())
                    {
                        musicDataStruct temp;
                        temp.filepath      = outPutStringHandle(getSongFromPlayList.value(2).toString());
                        temp.title         = outPutStringHandle(getSongFromPlayList.value(3).toString());
                        temp.singer        = outPutStringHandle(getSongFromPlayList.value(4).toString());
                        temp.album         = outPutStringHandle(getSongFromPlayList.value(5).toString());
                        temp.filetype      = outPutStringHandle(getSongFromPlayList.value(6).toString());
                        temp.size          = outPutStringHandle(getSongFromPlayList.value(7).toString());
                        temp.time          = outPutStringHandle(getSongFromPlayList.value(8).toString());

                        resList.append(temp);
                    }

                    if(0 == resList.size())
                    {
                        qDebug() << "歌单表中查询歌曲数量为0" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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

int MusicDataBase::changeSongOrderInPlayList(const QString& selectFilePath, const QString& destinationFilePath, const QString& playListName)
{
    //入参检查
    if(selectFilePath.isEmpty() || destinationFilePath.isEmpty() || playListName.isEmpty())
    {
        qDebug() << "入参为空" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;//入参为空
    }
    if(DB_OP_SUCC != checkPlayListExist(playListName))
    {
        qDebug() << "歌单在歌单表中不存在" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;//歌单在歌单表中不存在
    }
    if(DB_OP_SUCC != checkIfSongExistsInPlayList(selectFilePath, playListName)
    || DB_OP_SUCC != checkIfSongExistsInPlayList(destinationFilePath, playListName))
    {
        qDebug() << "歌曲在歌单表中不存在" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;//歌曲在歌单表中不存在
    }
    if(selectFilePath == destinationFilePath)//位置没变化
    {
        qDebug() << "位置没变化，不用处理" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return DB_OP_SUCC;//位置没变化，不用处理
    }

    //获取歌曲的index信息
    int oldIndex = 0, newIndex = 0;
    int getRes1 = getSongIndexFromPlayList(selectFilePath, playListName, oldIndex);
    int getRes2 = getSongIndexFromPlayList(destinationFilePath, playListName, newIndex);

    if(getRes1 != DB_OP_SUCC)
    {
        qDebug() << "歌曲原INDEX获取失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return getRes1;
    }
    if(getRes2 != DB_OP_SUCC)
    {
        qDebug() << "目标歌曲INDEX获取失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return getRes2;
    }

    if(oldIndex < newIndex)//靠前的位置拖拽到了靠后的位置
    {
        bool setRes = true;
        QSqlQuery setSongIndexFromLocal(m_database);
        QString setIndex = QString("update 'playlist_%1' set idIndex=0 WHERE filepath='%2'")
                                    .arg(inPutStringHandle(playListName))
                                    .arg(inPutStringHandle(selectFilePath));
        setRes &= setSongIndexFromLocal.exec(setIndex);

        if(true == setRes)
        {
            bool getRes = true;
            QSqlQuery getBetweenSongIndexFromLocal(m_database);
            QString setIndexs = QString("select * from 'playlist_%1' WHERE idIndex between '%2' and '%3'")
                                        .arg(inPutStringHandle(playListName)).arg(oldIndex).arg(newIndex);
            getRes &= getBetweenSongIndexFromLocal.exec(setIndexs);

            if(true == getRes)
            {
                bool updateIndexRes = true;
                while(getBetweenSongIndexFromLocal.next())
                {
                    int tempIndex           = getBetweenSongIndexFromLocal.value(1).toInt();
                    QString tempFilepath    = outPutStringHandle(getBetweenSongIndexFromLocal.value(2).toString());

                    QSqlQuery updateSongIndexFromLocal(m_database);
                    QString updateIndex = QString("update 'playlist_%1' set idIndex='%2' WHERE filepath='%3'")
                                                  .arg(inPutStringHandle(playListName)).arg(tempIndex-1).arg(inPutStringHandle(tempFilepath));
                    updateIndexRes &= updateSongIndexFromLocal.exec(updateIndex);
                }

                if(true == updateIndexRes)
                {
                    bool setRes2 = true;
                    QSqlQuery setSongIndexFromLocal2(m_database);
                    QString setIndex2 = QString("update 'playlist_%1' set idIndex='%2' WHERE filepath='%3'")
                                                  .arg(inPutStringHandle(playListName)).arg(newIndex).arg(inPutStringHandle(selectFilePath));
                    setRes2 &= setSongIndexFromLocal2.exec(setIndex2);

                    if(true == setRes2)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        return LIST_REORDER_ERR;
                    }
                }
                else
                {
                    return LIST_REORDER_ERR;
                }
            }
        }
    }
    else if(oldIndex > newIndex)//靠后的位置拖拽到了靠前的位置
    {
        bool setRes = true;
        QSqlQuery setSongIndexFromLocal(m_database);
        QString setIndex = QString("update 'playlist_%1' set idIndex=0 WHERE filepath='%2'")
                                   .arg(inPutStringHandle(playListName)).arg(inPutStringHandle(selectFilePath));
        setRes &= setSongIndexFromLocal.exec(setIndex);

        if(true == setRes)
        {
            bool getRes = true;
            QSqlQuery getBetweenSongIndexFromLocal(m_database);
            QString setIndexs = QString("select * from 'playlist_%1' WHERE idIndex between '%2' and '%3' order by idIndex desc")
                                        .arg(inPutStringHandle(playListName)).arg(newIndex+1).arg(oldIndex);
            getRes &= getBetweenSongIndexFromLocal.exec(setIndexs);

            if(true == getRes)
            {
                bool updateIndexRes = true;
                while(getBetweenSongIndexFromLocal.next())
                {
                    int tempIndex           = getBetweenSongIndexFromLocal.value(1).toInt();
                    QString tempFilepath    = outPutStringHandle(getBetweenSongIndexFromLocal.value(2).toString());

                    QSqlQuery updateSongIndexFromLocal(m_database);
                    QString updateIndex = QString("update 'playlist_%1' set idIndex='%2' WHERE filepath='%3'")
                                                  .arg(inPutStringHandle(playListName)).arg(tempIndex+1).arg(inPutStringHandle(tempFilepath));
                    updateIndexRes &= updateSongIndexFromLocal.exec(updateIndex);
                }

                if(true == updateIndexRes)
                {
                    bool setRes2 = true;
                    QSqlQuery setSongIndexFromLocal2(m_database);
                    QString setIndex2 = QString("update 'playlist_%1' set idIndex='%2' WHERE filepath='%3'")
                                                .arg(inPutStringHandle(playListName)).arg(newIndex+1).arg(inPutStringHandle(selectFilePath));
                    setRes2 &= setSongIndexFromLocal2.exec(setIndex2);

                    if(true == setRes2)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        return LIST_REORDER_ERR;
                    }
                }
                else
                {
                    return LIST_REORDER_ERR;
                }
            }
        }
    }
    else//位置没变
    {
        return DB_OP_SUCC;
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
        if(true == m_database.isValid())
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

            //历史列表中没有这首歌，直接添加
            if(SONG_NOT_FOUND == checkHistoryRes)
            {
                //历史列表中不存在该歌曲，添加该歌曲
                QSqlQuery addSongToHistory(m_database);
                QString addSongString = QString("insert into HistoryPlayList (filepath,title,singer,album,filetype,size,time) values('%1','%2','%3','%4','%5','%6','%7')").
                        arg(inPutStringHandle(temp.filepath)).
                        arg(inPutStringHandle(temp.title)).
                        arg(inPutStringHandle(temp.singer)).
                        arg(inPutStringHandle(temp.album)).
                        arg(inPutStringHandle(temp.filetype)).
                        arg(inPutStringHandle(temp.size)).
                        arg(inPutStringHandle(temp.time));
                queryRes = addSongToHistory.exec(addSongString);
                //插入歌曲时自增id和idIndex无法赋值，插入后取得自增id，给idIndex赋值
                int tempIndex = addSongToHistory.lastInsertId().toInt();
                bool setRes = true;
                QSqlQuery setSongIDFromLocal(m_database);
                QString setIndex = QString("update HistoryPlayList set idIndex='%1' WHERE filepath='%2'").
                        arg(tempIndex).arg(inPutStringHandle(temp.filepath));
                setRes &= setSongIDFromLocal.exec(setIndex);

                if(true == (queryRes&setRes))
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，添加失败！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                    return DB_OP_ADD_FAILED;
                }
            }
            else//历史列表中已存在该，歌曲，需要先删除再添加
            {
                int delHistoryRes = delMusicFromHistoryMusic(filePath);
                if(DB_OP_SUCC == delHistoryRes)
                {
                    //历史列表中不存在该歌曲，添加该歌曲
                    QSqlQuery addSongToHistory(m_database);
                    QString addSongString = QString("insert into HistoryPlayList (filepath,title,singer,album,filetype,size,time) values('%1','%2','%3','%4','%5','%6','%7')").
                            arg(inPutStringHandle(temp.filepath)).
                            arg(inPutStringHandle(temp.title)).
                            arg(inPutStringHandle(temp.singer)).
                            arg(inPutStringHandle(temp.album)).
                            arg(inPutStringHandle(temp.filetype)).
                            arg(inPutStringHandle(temp.size)).
                            arg(inPutStringHandle(temp.time));
                    queryRes = addSongToHistory.exec(addSongString);
                    //插入歌曲时自增id和idIndex无法赋值，插入后取得自增id，给idIndex赋值
                    int tempIndex = addSongToHistory.lastInsertId().toInt();
                    bool setRes = true;
                    QSqlQuery setSongIDFromLocal(m_database);
                    QString setIndex = QString("update HistoryPlayList set idIndex='%1' WHERE filepath='%2'").
                            arg(tempIndex).arg(inPutStringHandle(temp.filepath));
                    setRes &= setSongIDFromLocal.exec(setIndex);

                    if(true == (queryRes&setRes))
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        qDebug() << "数据库打开，添加失败！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                        return DB_OP_ADD_FAILED;
                    }
                }
                else
                {
                    return delHistoryRes;
                }
            }


        }
        else
        {
            qDebug() << "数据库无法打开，请重试！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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

                if(true == delRes)
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，删除失败！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                    return DB_OP_DEL_FAILED;
                }
            }
            else
            {
                qDebug() << "歌曲不存在！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return checkHistoryRes;
            }
        }
        else
        {
            qDebug() << "数据库无法打开，请重试！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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
        if(true == m_database.isValid())
        {
            int checkRes = checkIfSongExistsInHistoryMusic(filePath);

            if(checkRes == SONG_NOT_FOUND)
            {
                return SONG_NOT_FOUND;
            }
            else
            {
                QSqlQuery getSongInfoFromHistoryPlayList(m_database);

                QString getSongString = QString("select * from HistoryPlayList where filepath = '%1'").arg(inPutStringHandle(filePath));
                getRes &= getSongInfoFromHistoryPlayList.exec(getSongString);
                if(false == getRes)
                {
                    return DB_OP_GET_FAILED;
                }

                //判断是否找到了对应的歌曲
                if(getSongInfoFromHistoryPlayList.next())
                {
                    fileData.filepath    = outPutStringHandle(getSongInfoFromHistoryPlayList.value(2).toString());
                    fileData.title       = outPutStringHandle(getSongInfoFromHistoryPlayList.value(3).toString());
                    fileData.singer      = outPutStringHandle(getSongInfoFromHistoryPlayList.value(4).toString());
                    fileData.album       = outPutStringHandle(getSongInfoFromHistoryPlayList.value(5).toString());
                    fileData.filetype    = outPutStringHandle(getSongInfoFromHistoryPlayList.value(6).toString());
                    fileData.size        = outPutStringHandle(getSongInfoFromHistoryPlayList.value(7).toString());
                    fileData.time        = outPutStringHandle(getSongInfoFromHistoryPlayList.value(8).toString());

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
            qDebug() << "数据库无法打开，请重试！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
            return DB_UNCONNECT;
        }
    }

}

int MusicDataBase::getSongInfoListFromHistoryMusic(QList<musicDataStruct>& resList)
{
    if(true == m_database.isValid())
    {
        bool getRes = true;
        QSqlQuery getSongListFromHistoryMusic(m_database);
        QString getSongListString = QString("select * from HistoryPlayList order by idIndex desc");
        getRes = getSongListFromHistoryMusic.exec(getSongListString);

        if(true == getRes)
        {
            while(getSongListFromHistoryMusic.next())
            {
                musicDataStruct temp;
                temp.filepath = outPutStringHandle(getSongListFromHistoryMusic.value(2).toString());
                int curRes = getSongInfoFromHistoryMusic(temp.filepath, temp);

                if(DB_OP_SUCC == curRes)
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
        qDebug() << "歌曲未找到" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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

    if(true == m_database.isValid())
    {
//        QMutexLocker lockData( &m_mutex);  //加锁，函数执行完后自动解锁

        QSqlQuery getplayListFromLocalMusic(m_database);
        QString getplayListFromLocalMusicString = QString("select * from ListOfPlayList");
        getRes = getplayListFromLocalMusic.exec(getplayListFromLocalMusicString);

        if(false == getRes)
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

int MusicDataBase::renamePlayList(const QString& oldPlayListName, const QString& newPlayListName)
{
    if(oldPlayListName.isEmpty() || newPlayListName.isEmpty())
    {
        return INVALID_INPUT;
    }

    if("我喜欢" == oldPlayListName || "我喜欢" == newPlayListName)
    {
        return INVALID_INPUT;
    }
    int oldCheckRes = checkPlayListExist(oldPlayListName);
    if(DB_OP_SUCC == oldCheckRes)
    {
        int newCheckRes = checkPlayListExist(newPlayListName);
        if(LIST_NOT_FOUND == newCheckRes)
        {
            //sql支持table重命名
            bool queryRes = true;
            QSqlQuery queryRename(m_database);
            //重命名歌单
            QString renameListString = QString("ALTER TABLE 'playlist_%1' RENAME TO 'playlist_%2'").
                    arg(inPutStringHandle(oldPlayListName)).
                    arg(inPutStringHandle(newPlayListName));
            queryRes &= queryRename.exec(renameListString);
            if(true == queryRes)
            {
                //要把歌单列表中的对应旧歌单名删除，添加新歌单名
                int createRes = createNewPlayList(newPlayListName);
                if(DB_OP_SUCC == createRes)
                {
                    int delRes = delPlayList(oldPlayListName);
                    if(DB_OP_SUCC == delRes)
                    {
                        return DB_OP_SUCC;//歌单名表名和歌单列表中的名称全部改变
                    }
                    else
                    {
                        return LIST_RENAME_ERR;//歌单名已改变，但歌单表中同时存在新歌单名和旧歌单名
                    }
                }
                else
                {
                    return LIST_RENAME_ERR;//歌单名已改变，但歌单表中名未改变
                }
            }
            else
            {
                return LIST_RENAME_FAILED;
            }
        }
        else
        {
            if(DB_OP_SUCC == newCheckRes)
            {
                return LIST_EXISTS;
            }
            else
            {
                return newCheckRes;
            }
        }
    }
    else
    {
        return oldCheckRes;
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

        if(false == getRes)
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
            //该列表不存在
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
            if(true == m_database.isValid())
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

                bool addRes;
                //歌单列表中不存在该歌曲，添加该歌曲
                QSqlQuery addSongToHistory(m_database);
                QString addSongString = QString("insert into 'playlist_%1' (filepath,title,singer,album,filetype,size,time) values('%2','%3','%4','%5','%6','%7','%8')").
                        arg(inPutStringHandle(playListName)).
                        arg(inPutStringHandle(temp.filepath)).
                        arg(inPutStringHandle(temp.title)).
                        arg(inPutStringHandle(temp.singer)).
                        arg(inPutStringHandle(temp.album)).
                        arg(inPutStringHandle(temp.filetype)).
                        arg(inPutStringHandle(temp.size)).
                        arg(inPutStringHandle(temp.time));
                addRes = addSongToHistory.exec(addSongString);

                int tempIndex = addSongToHistory.lastInsertId().toInt();
                bool setRes = true;
                QSqlQuery setSongIDFromLocal(m_database);
                QString setIndex = QString("update 'playlist_%1' set idIndex='%2' WHERE filepath='%3'").
                        arg(inPutStringHandle(playListName)).arg(tempIndex).arg(inPutStringHandle(temp.filepath));
                setRes &= setSongIDFromLocal.exec(setIndex);

                if(true == (addRes&setRes))
                {
                    return DB_OP_SUCC;
                }
                else
                {
                    qDebug() << "数据库打开，添加失败！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                    return DB_OP_ADD_FAILED;
                }

            }
            else
            {
                qDebug() << "数据库无法打开，请重试！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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
//从歌单中添加歌曲到新建歌单，如果本地歌单中没有，同样插入本地歌单，使用musicDataStruct结构,输入数据必须有效，
int MusicDataBase::addNewSongToPlayList(const musicDataStruct& fileData,const QString& playListName)
{
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }
    if(fileData.title.isEmpty() || fileData.filepath.isEmpty())
    {
        qDebug() << "无效入参" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return INVALID_INPUT;
    }

    int checkListRes = checkIfSongExistsInPlayList(fileData.filepath,playListName);
    if(SONG_NOT_FOUND != checkListRes)
    {
        return checkListRes;
    }
    else
    {
        int checkLocalRes = checkIfSongExistsInLocalMusic(fileData.filepath);
        if(checkLocalRes == SONG_NOT_FOUND)
        {
            int addLocalRes;
            addLocalRes = addMusicToLocalMusic(fileData);
            if(DB_OP_SUCC == addLocalRes)
            {
                int addListRes;
                addListRes = addMusicToPlayList(fileData.filepath,playListName);
                return addListRes;
            }
            else
            {
                return addLocalRes;
            }
        }
        else if(checkLocalRes == DB_OP_SUCC)
        {
            int addListRes;
            addListRes = addMusicToPlayList(fileData.filepath,playListName);
            return addListRes;
        }
        else
        {
            return checkLocalRes;
        }
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
                    bool delRes;
                    //歌单列表中存在该歌曲，删除该歌曲
                    QSqlQuery delSongToHistory(m_database);
                    QString delSongString = QString("delete from 'playlist_%1' where filepath = '%2'").
                            arg(inPutStringHandle(playListName)).
                            arg(inPutStringHandle(filePath));
                    delRes = delSongToHistory.exec(delSongString);

                    if(true == delRes)
                    {
                        return DB_OP_SUCC;
                    }
                    else
                    {
                        qDebug() << "数据库打开， 删除失败！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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
                qDebug() << "数据库无法打开，请重试！！！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
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

int MusicDataBase::getSongIndexFromLocalMusic(const QString& filePath, int &songIndex)
{
    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
    {
        bool getRes = true;
        QSqlQuery getSongFromLocalMusic(m_database);
        QString getSongString = QString("select * from LocalMusic where filepath = '%1'").arg(inPutStringHandle(filePath));
        getRes = getSongFromLocalMusic.exec(getSongString);

        if(true == getRes)
        {
            if(getSongFromLocalMusic.next())
            {
                songIndex    = getSongFromLocalMusic.value(1).toInt();
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

int MusicDataBase::getSongIndexFromPlayList(const QString& filePath,const QString& playListName, int &songIndex)
{
    if(filePath.isEmpty())
    {
        return INVALID_INPUT;
    }

    if(true == m_database.isValid())
    {
        int checkRes = checkPlayListExist(playListName);
        if(checkRes == DB_OP_SUCC)
        {
            bool getRes = true;
            QSqlQuery getSongFromPlayList(m_database);
            QString getSongString = QString("select * from 'playlist_%1' where filepath = '%2'").
                    arg(inPutStringHandle(playListName)).arg(inPutStringHandle(filePath));
            getRes = getSongFromPlayList.exec(getSongString);

            if(true == getRes)
            {
                if(getSongFromPlayList.next())
                {
                    songIndex    = getSongFromPlayList.value(1).toInt();
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
            return checkRes;
        }

    }
    else
    {
        return DB_DISORDERD;
    }
}

int MusicDataBase::delSongFromEveryWhere(const QString& filePath)
{
    //从历史歌单中删除该歌
    int checkHistoryRes = checkIfSongExistsInHistoryMusic(filePath);
    if(DB_OP_SUCC == checkHistoryRes)
    {
        int delHistoryRes = delMusicFromHistoryMusic(filePath);
        if(DB_OP_SUCC != delHistoryRes)
        {
            return delHistoryRes;
        }
        else
        {}
    }
    else
    {
        if(SONG_NOT_FOUND != checkHistoryRes)
        {
            return checkHistoryRes;
        }
        else
        {}
    }

    //从总歌单中删除该歌
    int checkLocalRes = checkIfSongExistsInLocalMusic(filePath);
    if(DB_OP_SUCC == checkLocalRes)
    {
        int delLocalRes = delMusicFromLocalMusic(filePath);
        if(DB_OP_SUCC != delLocalRes)
        {
            return delLocalRes;
        }
        else
        {}
    }
    else
    {
        if(SONG_NOT_FOUND != checkLocalRes)
        {
            return checkLocalRes;
        }
        else
        {}
    }

    QStringList temp;
    //查询当前已有歌单列表
    int getPlayListRes = getPlayList(temp);
    if(DB_OP_SUCC == getPlayListRes)
    {
        foreach (auto tempList, temp)
        {
            //检查歌曲是否在歌单列表中存在
            int checkPlayListRes = checkIfSongExistsInPlayList(filePath, tempList);
            if(DB_OP_SUCC == checkPlayListRes)
            {
                //从新建歌单中删除歌曲，使用歌曲filePath,歌单名title值，输入数据必须有效，
                int delMusicFromPlayListRes = delMusicFromPlayList(filePath, tempList);
                if(DB_OP_SUCC != delMusicFromPlayListRes)
                {
                    return delMusicFromPlayListRes;
                }
                else
                {}
            }
            else
            {
                if(SONG_NOT_FOUND != checkPlayListRes)
                {
                    return checkPlayListRes;
                }
                else
                {}
            }
        }
    }
    else
    {
        return getPlayListRes;
    }
    //从历史歌单，总歌单，各新建歌单中都删除后，返回成功。
    return DB_OP_SUCC;
}

//删除多首歌曲
int MusicDataBase::delMultiSongs(const QString &playListName, const QStringList &songsList)
{
    //入参检查
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }
    if(0 == songsList.size())
    {
        return INVALID_INPUT;
    }
    foreach (auto songName, songsList)
    {
        if(songName.isEmpty())
        {
            return INVALID_INPUT;
        }
    }
    if("LocalMusic" == playListName || "HistoryPlayList" == playListName)
    {

    }
    else
    {
        int playListCheckRes = checkPlayListExist(playListName);
        if(DB_OP_SUCC != playListCheckRes)
        {
            return playListCheckRes;
        }
    }

    foreach (auto songName, songsList)
    {
        int checkIfSongExists = OUT_OF_RESULT;
        if("LocalMusic" == playListName)
        {
            checkIfSongExists = checkIfSongExistsInLocalMusic(songName);
            if(SONG_NOT_FOUND == checkIfSongExists)
            {
                continue;
            }
        }
        else if("HistoryPlayList" == playListName)
        {
            checkIfSongExists = checkIfSongExistsInHistoryMusic(songName);
            if(SONG_NOT_FOUND == checkIfSongExists)
            {
                continue;
            }
        }
        else
        {
            checkIfSongExists = checkIfSongExistsInPlayList(songName, playListName);
            if(SONG_NOT_FOUND == checkIfSongExists)
            {
                continue;
            }
        }


        if(DB_OP_SUCC == checkIfSongExists)
        {
            int delRes = OUT_OF_RESULT;
            if("LocalMusic" == playListName)
            {
                delRes = delMusicFromLocalMusic(songName);
            }
            else if("HistoryPlayList" == playListName)
            {
                delRes = delMusicFromHistoryMusic(songName);
            }
            else
            {
                delRes = delMusicFromPlayList(songName, playListName);
            }

            if(delRes != DB_OP_SUCC)
            {
                return delRes;
            }
            else
            {
                continue;
            }
        }
        else
        {
            return checkIfSongExists;
        }
    }
    return DB_OP_SUCC;
}

int MusicDataBase::checkIfPlayListIsEmpty(const QString& playListName)
{
    if(playListName.isEmpty())
    {
        return INVALID_INPUT;
    }
    int playListCheckRes = checkPlayListExist(playListName);
    if(DB_OP_SUCC != playListCheckRes)
    {
        return playListCheckRes;
    }
    else
    {
        if(true == m_database.isValid())
        {
            bool getRes = true;
            int count = 0;
            QSqlQuery getSongFromPlayList(m_database);
            QString getSongString = QString("select count(*) from 'playlist_%1'").arg(inPutStringHandle(playListName));
            getRes = getSongFromPlayList.exec(getSongString);
            if(true == getRes)
            {
                if(getSongFromPlayList.next())
                {
                    count = getSongFromPlayList.value(0).toInt();
                    if(count > 0)
                    {
                        return LIST_NOT_EMPTY;
                    }
                    else
                    {
                        return LIST_IS_EMPTY;
                    }
                }
                else
                {
                    qDebug() << "数据库中查无此歌！" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                    return DB_DISORDERD;
                }
            }
            else
            {
                return DB_OP_GET_FAILED;
            }
        }
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
