#include "kylinmuisc.h"
#include "myapplication.h"
#include "changelistwid.h"
#include "songitem.h"

MainWid * KylinMuisc::main_window = 0;

KylinMuisc::KylinMuisc(const QString &arch, const QString &snap, QObject *parent)
    : QObject(parent)
//    , m_controllerWorker(controller)
    , m_moveGui(false)
    , m_resizeGui(false)
    , m_arch(arch)
    , m_snap(snap)
{
    wid = new MusicListWid;
    m_thread = new QThread;
    wid->moveToThread(m_thread);
    connect(m_thread, &QThread::started, this, [=] ()
    {
        if(!m_filesToPlay.isEmpty())
        {
            processArgs(m_filesToPlay);
            qDebug()<<"m_filesToPlay m_filesToPlay m_filesToPlay: "<<m_filesToPlay;
        }
//        gui()->show();
    });
    m_thread->start();
}
KylinMuisc::~KylinMuisc()
{
    if(wid)
    {
        wid->deleteLater();
    }
    if (m_thread) {
        m_thread->quit();
        m_thread->wait(2000);
    }

    if (main_window != 0) {
        deleteGUI();
    }
}

void KylinMuisc::deleteGUI()
{
    delete main_window;
    main_window = 0;
}

KylinMuisc::ExitCode KylinMuisc::processArgs(QStringList args)
{
    qDebug()<<"args : "<<args;
    QSqlQuery query;
    QString action;
    bool show_help = false;
    bool add_to_playlist = false;
//    QString action; // Action to be passed to running instance
//    bool add_to_playlist = false;
//    if(wid->musicInfoWidget->count() > 0)
//    {

//    }
    qDebug()<<"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL";
    for(int i = 1; i < args.size(); i++)
    {
        qDebug()<<"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL";
//        QString argument = args[i];
        qDebug()<<"args : "<<args;
//        qDebug()<<"argument : "<<argument;
        Dir = QDir::toNativeSeparators(args.at(i));
        musicPath = args.at(i);
        qDebug()<<"musicPath : "<<musicPath;
        if(!musicPath.isEmpty())
        {
            qDebug()<<"musicPath : "<<musicPath;
            bool songExists = false;
            for(QString &songPlaylist:wid->allmusic)
            {
                qDebug()<<"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL";
                if(musicPath == songPlaylist)    //通过路径判断歌曲是否存在
                {
                    qDebug()<<"歌曲已存在 (通过路径判断歌曲是否存在)";
//                        qDebug()<<"songPlaylist : "<<songPlaylist;
                    songExists = true;
                    break;
                }
            }
            if(songExists)
            {
                continue;
            }

            fileInfo.setFile(musicPath);

            musicType = fileInfo.suffix();        //文件类型

            qint64 musicFileSize = fileInfo.size();
            if(musicFileSize/1024)
            {
                if(musicFileSize/1024/1024)
                {
                    if(musicFileSize/1024/1024/1024)
                    {
                        musicSize = QString::number(musicFileSize/1024/1024/1024,10)+"GB";
                    }
                    else
                        musicSize = QString::number(musicFileSize/1024/1024,10)+"MB";
                }
                else
                    musicSize = QString::number(musicFileSize/1024,10)+"KB";
            }
            else
                musicSize = QString::number(musicFileSize,10)+"B";
            byteArray = musicPath.toLocal8Bit();

            if(musicPath.split(".").last() != "amr" && musicPath.split(".").last() != "mmf")
            {
                TagLib::FileRef f(byteArray.data());
                if(f.isNull())
                {
                    //can't read this music;
                    continue;
                }
                TagLib::PropertyMap propertyMap = f.file() ->properties();

                musicName = propertyMap["TITLE"].toString().toCString(true);
                if(musicName.isEmpty())
                    musicName = fileInfo.baseName();
                musicSinger = propertyMap["ARTIST"].toString().toCString(true);
                if(musicSinger.isEmpty())
                    musicSinger = "未知歌手";
                musicAlbum = propertyMap["ALBUM"].toString().toCString(true);
                if(musicAlbum.isEmpty())
                    musicAlbum = "未知专辑";
                TagLib::AudioProperties *properties = f.audioProperties();

                int seconds = properties->length() % 60;
                int minutes = (properties->length() - seconds) / 60;
                musicTime = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
            }
            else
            {
                TagLib::FileRef f(byteArray.data());

                    musicName = fileInfo.baseName();

                    musicSinger = "未知歌手";

                    musicAlbum = "未知专辑";

                  qDebug() << "musicSize" << musicSize;
            }
            QListWidgetItem *listItem = new QListWidgetItem(wid->musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            wid->musicInfoWidget->setItemWidget(listItem,songitem1);
            songitem1->song_singer_albumText(musicName,musicSinger,musicAlbum); //歌曲名称 歌手 专辑
            songitem1->songTimeLabel->setText(musicTime); //时长

            wid->localModel->setTable("LocalMusic");
            wid->localModel->select();

            int id = 0;
            if (wid->localModel->rowCount() != 0)
            {
                int num = wid->localModel->data(wid->localModel->index(wid->localModel->rowCount()-1,0)).toInt();
                id = num + 1;
            }

            wid->allmusic.append(musicPath);
            wid->localAllMusicid.append(QString::number(id));
            wid->PlayList->addMedia(QUrl::fromLocalFile(musicPath));
            query.exec(QString("insert into LocalMusic (id,musicname,filepath,singer,album,type,size,time) values (%1,'%2','%3','%4','%5','%6','%7','%8')")
                       .arg(id).arg(musicName).arg(musicPath).arg(musicSinger).arg(musicAlbum).arg(musicType).arg(musicSize).arg(musicTime));

            wid->songNumberLabel->setText(tr("A total of")+QString::number(wid->musicInfoWidget->count())+tr("The first"));
        }
    }
    wid->Music->setPlaylist(wid->PlayList);
//    qDebug()<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
//    for (int n=0; n < m_filesToPlay.count(); n++)
//    {
//        qDebug("KylinMusic::processArgs: m_filesToPlay[%d]: '%s'", n, m_filesToPlay[n].toUtf8().data());
//    }

//    // Single instance
    MyApplication * a = MyApplication::instance();
    if (a->isRunning())
    {
        a->sendMessage("Hello");

        if (!action.isEmpty())
        {
            a->sendMessage("action " + action);
        }
        else
        {
            if (!m_subtitleFile.isEmpty())
            {
                a->sendMessage("load_sub " + m_subtitleFile);
            }

            if (!m_mediaTitle.isEmpty())
            {
                a->sendMessage("media_title " + m_filesToPlay[0] + " <<sep>> " + m_mediaTitle);
            }

            if (!m_filesToPlay.isEmpty())
            {
                QString command = "open_files";
                if (add_to_playlist) command = "add_to_playlist";
                a->sendMessage(command +" "+ m_filesToPlay.join(" <<sep>> "));
            }
        }
        return NoError;
    }
    qDebug()<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
    return KylinMuisc::NoExit;
}
