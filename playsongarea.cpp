#include "playsongarea.h"

PlaySongArea::PlaySongArea(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(68);

    setStyleSheet("QWidget{Background-color:#FFFFFF;}");


    initWidget();
}


void PlaySongArea::initWidget()
{
    h_mainLayout = new QHBoxLayout(this);
    QVBoxLayout *v_mainLayout = new QVBoxLayout(this);

    lastBtn = new QPushButton;
    lastBtn->setFixedSize(18,18);
    lastBtn->setCursor(Qt::PointingHandCursor);
    lastBtn->setToolTip(" 上一首");
    lastBtn->setIcon(QIcon(":/img/default/lastsong.png"));
    lastBtn->setIconSize(QSize(18,18));
    lastBtn->setStyleSheet("QPushButton{border-radius:15px}");

    playBtn = new QPushButton(this);
    playBtn->setFixedSize(36,36);
    playBtn->setCursor(Qt::PointingHandCursor);
    playBtn->setToolTip(" 播放");
    playBtn->setIcon(QIcon(":/img/default/play2.png"));
    playBtn->setIconSize(QSize(36,36));
    playBtn->setStyleSheet("QPushButton{border-radius:17px}");



    nextBtn = new QPushButton;
    nextBtn->setFixedSize(18,18);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setToolTip(" 下一首");
    nextBtn->setIcon(QIcon(":/img/default/nextsong_b@2x.png"));
    nextBtn->setIconSize(QSize(18,18));
    nextBtn->setStyleSheet("QPushButton{border-radius:15px}");



//    startTimeLabel = new QLabel("00:00");
//    startTimeLabel->setFixedSize(37,15);

//    playTimeSlider = new QSlider;
//    playTimeSlider->setOrientation(Qt::Horizontal);
//    playTimeSlider->setFixedHeight(14);
//    playTimeSlider->setRange(0,200);
//    playTimeSlider->setSingleStep(1);
//    playTimeSlider->setStyleSheet("QSlider::groove:horizontal{height: 4px;background: rgb(102,183,255);}\
//                                   QSlider::add-page:horizontal{background:#c2c2c4;}\
//                                   QSlider::handle:horizontal{width: 14px;background: url(:/images/BottomPlay/curTimeHandle.png);margin: -5 0 -5 0;}");

//    playTimeSlider->installEventFilter(this);

//    endTimeLabel = new QLabel("00:00");
//    endTimeLabel->setFixedSize(37,15);

//    m_controlFrame = new QWidget;
//    m_controlFrame->setStyleSheet("border:none");
//    m_hControlLayout = new QHBoxLayout(m_controlFrame);
//    m_hControlLayout->setSpacing(10);
//    m_hControlLayout->setMargin(10);
//    m_hControlLayout->addWidget(startTimeLabel,Qt::AlignVCenter);
//    m_hControlLayout->addWidget(playTimeSlider,Qt::AlignVCenter);
//    m_hControlLayout->addWidget(endTimeLabel,Qt::AlignVCenter);




    volumeBtn = new QPushButton;
    volumeBtn->setFixedSize(16,16);
    volumeBtn->setCursor(Qt::PointingHandCursor);
//    volumeBtn->setToolTip(" 静音 ");
    volumeBtn->setToolTip(" 功能未实现 ");
    volumeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/vol_large.png)}");
//    volumeBtn->hide();

//    volumeSlider = new QSlider;
//    volumeSlider->setOrientation(Qt::Horizontal);
//    volumeSlider->setFixedSize(100,12);
//    volumeSlider->setToolTip(" 音量调节");
//    volumeSlider->setMinimum(0);
//    volumeSlider->setMaximum(100);
//    volumeSlider->setSingleStep(1);
//    volumeSlider->setStyleSheet("QSlider::groove:horizontal{height: 4px;background: rgb(102,183,255);}\
//                                 QSlider::add-page:horizontal{background:#c2c2c4;}\
//                                 QSlider::handle:horizontal{width: 12px;background: url(:/img/default/vol_large.png);margin: -4 0 -4 0}");
//    volumeSlider->installEventFilter(this);

    likeMusicBtn = new QPushButton;
    likeMusicBtn->setFixedSize(16,16);
    likeMusicBtn->setCursor(Qt::PointingHandCursor);
//    likeMusicBtn->setToolTip(" 我喜欢");
    likeMusicBtn->setToolTip(" 功能未实现");

    likeMusicBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/loveblack.png)}");



    playModeBtn = new QPushButton;
    playModeBtn->setFixedSize(16,16);
    playModeBtn->setCursor(Qt::PointingHandCursor);
    playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/sequence.png);}"
                               "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}");
    playModeBtn->setToolTip(" 顺序播放 ");


//    lyricBtn = new QPushButton;
//    lyricBtn->setFixedSize(16,16);
//    lyricBtn->setCursor(Qt::PointingHandCursor);
//    lyricBtn->setToolTip(" 打开歌词 ");
//    lyricBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/lyric.png)}");

    listBtn = new QPushButton;   //历史播放列表
    listBtn->setFixedSize(16,16);
    listBtn->setCursor(Qt::PointingHandCursor);
    listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/songlist.png)}");
    listBtn->setToolTip(" 功能未实现 ");




    bottomLeftLabel = new QLabel(this);
    bottomLeftLabel->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *timeAndNameLayout = new QVBoxLayout(this);
    songNameofNowPlaying = new QLabel(this);
    QWidget *timeAndNameWidget = new QWidget(this);


    songNameofNowPlaying->setText("正在播放：" + MusicName);




    songNameofNowPlaying->setStyleSheet("width:56px;height:14px;font-size:14px;color:#303133;line-height:14px;");

    songNameofNowPlaying->hide();
    timeAndNameLayout->addWidget(songNameofNowPlaying);
    timeAndNameLayout->addWidget(bottomLeftLabel);

    timeAndNameLayout->setMargin(0);
    timeAndNameLayout->setSpacing(5);

    timeAndNameWidget->setLayout(timeAndNameLayout);




    h_mainLayout->addWidget(timeAndNameWidget,0,Qt::AlignLeft);
    h_mainLayout->addWidget(lastBtn,Qt::AlignCenter);
    h_mainLayout->addSpacing(10);
    h_mainLayout->addWidget(playBtn,Qt::AlignCenter);
    h_mainLayout->addSpacing(10);
    h_mainLayout->addWidget(nextBtn,Qt::AlignCenter);
//    h_mainLayout->addWidget(m_controlFrame,Qt::AlignVCenter);

    h_mainLayout->addWidget(volumeBtn,0,Qt::AlignRight);
//    h_mainLayout->addWidget(volumeSlider,Qt::AlignVCenter);
    h_mainLayout->addWidget(likeMusicBtn,Qt::AlignRight);
    h_mainLayout->addWidget(playModeBtn,Qt::AlignRight);
    h_mainLayout->addWidget(listBtn,Qt::AlignRight);
    h_mainLayout->setMargin(10);
    h_mainLayout->setSpacing(10);

//    connect(playBtn,SIGNAL(clicked(bool)),this,SLOT(play_Song()));   //播放歌曲
//    connect(playBtn,SIGNAL(clicked(bool)),this,SLOT(playSong(bool)));   //播放歌曲

}
//void PlaySongArea::playSong(bool)
//{
//    if(playstates)
//    {
//        playBtn->setFixedSize(36,36);
//        playBtn->setCursor(Qt::PointingHandCursor);
//        playBtn->setToolTip(" 播放");
//        playBtn->setIcon(QIcon(":/img/default/play2.png"));
//        playBtn->setIconSize(QSize(36,36));
//        playBtn->setStyleSheet("QPushButton{border-radius:17px}");
//        this->PauseSong();
//        playstates = false;

//    }
//    else
//    {
//        playBtn->setFixedSize(36,36);
//        playBtn->setCursor(Qt::PointingHandCursor);
//        playBtn->setToolTip(" 暂停");
//        playBtn->setIcon(QIcon(":/img/default/pause2.png"));
//        playBtn->setIconSize(QSize(36,36));
//        playBtn->setStyleSheet("QPushButton{border-radius:17px}");
////        this->playSong(bool);
//        playstates = true;
//    }
//}

void PlaySongArea::play_Song()   //播放和暂停
{
    if(Music ->state() == QMediaPlayer::PlayingState)
    {
        qDebug()<<"暂停 暂停";
        Music->pause();
    }
    else
    {
        qDebug()<<"播放 播放";
        Music->play();
    }
}

