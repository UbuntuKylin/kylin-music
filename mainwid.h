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

#ifndef MAINWID_H
#define MAINWID_H

#define KYLINRECORDER "org.kylin-music-data.settings"
#define FITTHEMEWINDOW "org.ukui.style"


#include <QGSettings>

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QTableWidgetItem>
#include <QPropertyAnimation>
#include <QSettings>
#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QSqlTableModel>
#include <QTime>
#include <QMessageBox>
#include <QMediaMetaData>
#include <QContextMenuEvent>
//#include <QApplication>
#include <QStandardPaths>
#include <QShortcut>
#include <QMessageBox>
#include <QRect>
#include <QTimer>
#include <QLabel>
#include <QtDBus>
//窗口显示在屏幕中心
#include <QApplication>
#include <QScreen>
//窗体阴影
#include <QPainter>

#include "titlebar.h"
#include "sidebar.h"
#include "changelistwid.h"
#include "playsongarea.h"
#include "slider.h"
#include "miniwidget.h"
#include "allpupwindow.h"
#include "daemonipcdbus.h"
#include "kylinmuisc.h"

#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib.h>
#include <taglib/tpropertymap.h>
//#include <taglib/mpeg/mpegfile.h>
//#include <taglib/mpeg/id3v2/id3v2tag.h>
//#include <taglib/mpeg/id3v2/frames/attachedpictureframe.h>

#include <unistd.h>
#include <fcntl.h>
#include <exception>
#include <QDBusInterface>
#include <QDBusConnection>

struct MusicPath
{
    QString path;

    bool ischecked;
};

class MainWid : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.kylin_music.play")
public:
    void Single(QString path);
    void promptMessage();
private:
//    void paintEvent(QPaintEvent *event);
//    QString g_file_path;
public:
    MainWid(QString str, QWidget *parent=nullptr);
//    MainWid(QWidget *);
    ~MainWid();
//    QString getMp3FileName(QString sqlName);
    void updatalistwidget(int value);//更新listWidget
    void updataplaylistwidget(int value);//更新playlistWidget
    void updateSongPlaying();
    void slot_showMiniWidget();//迷你模式
    void slot_closeMiniWidget();
    void slot_recoverNormalWidget();
    void changeDarkTheme();  //切换深浅色主题
    void changeLightTheme();
    void mousePressEvent(QMouseEvent *event);
    void get_historyplaylist_information();  //保存历史记录
    void local_Music();      //判断本地播放列表中歌曲是否存在
    void new_PlayList();     //判断本地歌单歌曲是否存在
    void albumCover_local_playlist(); //本地和歌单默认封面
    void local_playInformation();     //本地播放信息
    void playlist_playInformation();  //歌单播放信息
    void albumCover_local();          //本地专辑
    void albumCover_playlist();       //歌单专辑

QGSettings *themeData = nullptr;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    // 键盘响应事件
    void keyPressEvent(QKeyEvent *event);
    int kylin_music_play_request(QString path);

public slots:
//    void playSong(bool);
    void play_Song();   //播放和暂停
    void on_musicInfoWidget_customContextMenuRequested(const QPoint &pos);  //歌曲列表右键菜单
    void on_sidebarWidget_customContextMenuRequested(const QPoint &pos);    //侧边栏歌单区域右键菜单
//    void setHsliderPosition();      //添加歌曲时记住当前播放的位置
//    void contextMenuEvent(QContextMenuEvent *);
    void playOrPauseAct();    //右键播放
    void playNextSongAct();   //右键下一首

    void getSongInfoAct();    //歌曲信息
    void on_listWidget_doubleClicked(QListWidgetItem *item);         //双击本地音乐播放playlist
    void on_musicListChangeWid_doubleClicked(QListWidgetItem *item); //双击歌单播放
    void Music_stateChang(QMediaPlayer::State state);//播放状态改变
    void Music_playlist_stateChang(QMediaPlayer::State state);
    void on_lastBtn_clicked();             //上一首
//    void on_playlist_lastBtn_clicked();  //上一首
    void on_nextBtn_clicked();             //下一首
//    void on_playlist_nextBtn_clicked();  //下一首
    void positionChange(qint64 position);  //更新播放位置
    void durationChange(qint64 duration);  //更新播放进度
    void currentMediaChanged(QMediaContent content);  //按播放模式结束一段音乐自动输出封面、正在播放
    void playlist_positionChange(qint64 position);
    void playlist_durationChange(qint64 duration);
//    void playlist_currentMediaChanged(QMediaContent content);
    void setPosition(int position);
//    void playlist_setPosition(int position);
    bool eventFilter(QObject *obj, QEvent *event);   //鼠标滑块点击  事件过滤器
    void add_music_to_songlist(QAction *listact);    //添加到歌单
    void deleteMusicFromLocalList(); //从本地音乐删除
//    void deleteMusicFromSongList();  //从歌单删除音乐
    void deleteThisSongList();
    void showRenameDlg();
    void renameThisSongList();       //重命名歌单
    void promptThisSongList();       //歌单提示信息
    void renameSongListCon();
    void promptRenamePlayList();     //重命名歌单提示信息
    void promptRemovePlayList();     //删除歌单提示信息

    // 拖动进度条
    void slidePress();
//    void slideMove(int position);
    void slideRelease();
//    void playlist_slidePress();
////    void slideMove(int position);
//    void playlist_slideRelease();
    void PlayModeChanged(); //播放模式

    void initSystemTray();
    void showBeforeList();  //显示历史播放列表
    void show_volumeBtn();  //音量显示
    void changeVolume(int values);
    void addvSlider_slot();
    void subvSlider_slot();
    void showAboutWidget(); //显示关于界面
    void nullMusicWidgetAddSong(); // 空页面添加歌曲
    // 空页面添加文件夹
    void nullMusicWidgetAddFile();
    void add_nullMusicWidgetAddFile();
    void clear_HistoryPlayList();  //清除历史记录
    void showSearchResultWidget(); //显示搜索页面
    void hideSearchResultWidget(); //隐藏搜索页面
private:
//    bool isStartPlay = false;      //默认不播放媒体
public:
    QTimer *promptMessageTimer;
    QLabel *promptMessageLabel;
    void showPromptMessage();   //显示歌曲不存在的提示信息
    void closePromptMessage();

signals:
    void addFile(const QStringList &addFile);  //发送拖拽添加歌曲
//    void musicDbus(QString path);
//    void fromFilemanager(const QStringList &addFile); //拖拽添加歌曲信号
//private slots:
////    void addFile(const QStringList &addFile);
public slots:
//    void addFile(const QStringList &addFile);  //拖拽添加歌曲

//protected:
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dragLeaveEvent(QDragLeaveEvent *event);
//    void dropEvent(QDropEvent *event);

public:
    //添加文件夹
    QStringList AllDirList;
    QStringList DirList; // all checked dir
    QStringList SongDirPath;
    QDir matchDir;
    QStringList matchMp3Files;
    QString mp3Name;

    QFileInfo fileInfo;
    QByteArray bytes;
    QString titleStr;
    QString artistStr;
    QString albumStr;
    QString timeStr;
    QString mp3Size;
    QString type;
    QString MD5Str;
    QStringList MD5List;

    QString argName;

    QList<MusicPath> MusicPathList;
public:
    QString Dir;
    QString musicPath;

    QByteArray byteArray;
    QString musicName;
    QString musicSinger;
    QString musicAlbum;
    QString musicTime;
    QString musicSize;
    QString musicType;

private:
    // 用户手册功能
    DaemonIpcDbus *mDaemonIpcDbus;

    QWidget *mainWidget;
    QWidget *rightWid;
    TitleBar *myTitleBar;
    SideBar *mySideBar;
    ChangeListWid *nullMusicWidget;
    PlaySongArea *myPlaySongArea;
    SongInfoWidget *mySongInfoWidget;
    //文件拖拽功能

    QVBoxLayout *rightlayout;
    QSqlTableModel *model;
    QSqlTableModel *model_1;
    QMenu *Menu;
    QAction *playAct;   //右键播放
    QAction *nextAct;   //右键下一首
    QAction *likeAct;   //右键添加到我喜欢
    QAction *deleAct;   //右键从歌曲列表中删除
    QAction *listAct;   //右键添加到歌单
//    QAction *lookAct; //右键查看本地文件
    QAction *songAct;   //右键歌曲信息

    Slider *hSlider;
    Slider *vSlider;
    QWidget *vSliderWid;
    QHBoxLayout *HLayout;
    int moved = 0;

    miniWidget *m_MiniWidget;
//   SongItem *mySongItem;

    //侧边栏歌单列表右键
    QMenu *sideMenu;
    QAction *actionPlay;
    QAction *actionRename;
    QAction *actionDelete;

   //歌曲播放动效和高亮相关

//   int lastPlayIndex = 0;
    void currentPlayHighlight();
    // 判断删除歌曲时跳过高亮
    bool skipPlayHighlight = false;
//    // 判断删除歌单歌曲时跳过高亮
//    bool skipPlaylistHighlight = false;
    AllPupWindow *aboutWidget;
    int playMode = 0;

public:
    void processArgs(QStringList args);
private:
    void deleteConfig();
    static MainWid *main_wid;
    QStringList m_filesToPlay;
    QString m_subtitleFile;
    QString m_mediaTitle; //!< Force a title for the first file
    // Change position and size
    bool m_moveGui;
    QPoint m_guiPosition;
    bool m_resizeGui;
    QSize m_guiSize;
    QString m_arch;
    QString m_snap;
};
#endif // MAINWID_H
