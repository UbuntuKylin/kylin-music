#include "playsongarea.h"
#include "base/widgetstyle.h"
//#include "xatom-helper.h"
#include "base/musicDataBase.h"

PlaySongArea::PlaySongArea(QWidget *parent) : QWidget(parent)
{
    resize(750,88);
    setAttribute(Qt::WA_StyledBackground,true);
    initWidget();
    initConnect();
    playcolor();
}

void PlaySongArea::initWidget()
{
    QVBoxLayout *m_vmainLayout = new QVBoxLayout(this);
    QHBoxLayout *m_hmainLayout = new QHBoxLayout();
    m_mainLayout = new QHBoxLayout();

    preBtn = new QPushButton;
    preBtn->setFixedSize(18,18);
    preBtn->setCursor(Qt::PointingHandCursor);
    preBtn->setToolTip(tr("Previous"));

    playBtn = new QPushButton(this);
    playBtn->setFixedSize(36,36);
    playBtn->setCursor(Qt::PointingHandCursor);
//    playBtn->setToolTip("播放");
//    playBtn->setToolTip(tr("play"));

    nextBtn = new QPushButton;
    nextBtn->setFixedSize(18,18);
    nextBtn->setCursor(Qt::PointingHandCursor);
//    nextBtn->setToolTip("下一首");
    nextBtn->setToolTip(tr("Next"));

    volumeBtn = new QPushButton(this);
    volumeBtn->setFixedSize(16,16);
    volumeBtn->setCursor(Qt::PointingHandCursor);
    volumeBtn->setIcon(QIcon::fromTheme("audio-volume-high-symbolic"));
    volumeBtn->setProperty("isWindowButton", 0x1);
    volumeBtn->setProperty("useIconHighlightEffect", 0x2);
    volumeBtn->setFlat(true);
    volumeBtn->setToolTip(tr("Volume"));    //音量

    m_volSliderWid = new SliderWidget(this);
//    MotifWmHints hint;
//    hint.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
//    hint.functions = MWM_FUNC_ALL;
//    hint.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(m_volSliderWid->winId(), hint);
    m_volSliderWid->hide();

    m_playBackModeWid = new PlayBackModeWidget(this);
//    MotifWmHints hints;
//    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
//    hints.functions = MWM_FUNC_ALL;
//    hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(m_playBackModeWid->winId(), hints);
    m_playBackModeWid->hide();

    MusicSlider *hSlider = new MusicSlider(this);

    favBtn = new QPushButton;
    favBtn->setFixedSize(16,16);
    favBtn->setCheckable(true); //按钮是否是可点击状态，默认不点击
    favBtn->setChecked(false);
    favBtn->setCursor(Qt::PointingHandCursor);
    favBtn->setToolTip(tr("Favourite"));

    //播放模式
    playModeBtn = new QPushButton;
    playModeBtn->setFixedSize(16,16);
    playModeBtn->setCursor(Qt::PointingHandCursor);
    playModeBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
    playModeBtn->setProperty("isWindowButton", 0x1);
    playModeBtn->setProperty("useIconHighlightEffect", 0x2);
    playModeBtn->setFlat(true);
    playModeBtn->setToolTip(tr("Loop"));   //列表循环

    //历史播放列表
    listBtn = new QPushButton;
    listBtn->setFixedSize(16,16);
    listBtn->setCheckable(true); //按钮是否是可点击状态，默认不点击
//    listBtn->setChecked(false);    //只可检查按钮是否是点击状态，保存点击的状态
    listBtn->setCursor(Qt::PointingHandCursor);
    listBtn->setToolTip(tr("Play List"));

    coverPhotoLabel = new QLabel(this);
    coverPhotoLabel->setFixedSize(40,40);

    playingLabel = new QLabel(this);
    playingLabel->setFixedHeight(20);
    playingLabel->setText(tr("Kylin Music"));

    timeLabel = new QLabel(this);
    timeLabel->setText(tr("00:00/00:00"));

    QWidget *letfWid = new QWidget(this);
    QWidget *centreWid = new QWidget(this);
    QWidget *rightWid = new QWidget(this);

    QHBoxLayout *leftLayout = new QHBoxLayout(letfWid);

    QWidget *vWidget = new QWidget(letfWid);
    vWidget->setFixedHeight(40);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(playingLabel,0,Qt::AlignVCenter);
    vLayout->addSpacing(8);
    vLayout->addWidget(timeLabel,0,Qt::AlignVCenter);
    vLayout->addStretch();
    vLayout->setContentsMargins(8,3,0,3);
    vLayout->setMargin(0);
    vWidget->setLayout(vLayout);


    leftLayout->addWidget(coverPhotoLabel,Qt::AlignVCenter);
    leftLayout->addWidget(vWidget,Qt::AlignVCenter);
    leftLayout->setContentsMargins(30,5,0,14);
//    leftLayout->setMargin(0);

    QHBoxLayout *playLayout = new QHBoxLayout(centreWid);
    playLayout->addStretch();
    playLayout->addWidget(preBtn,Qt::AlignHCenter);
    playLayout->addSpacing(12);
    playLayout->addWidget(playBtn,Qt::AlignHCenter);
    playLayout->addSpacing(12);
    playLayout->addWidget(nextBtn,Qt::AlignHCenter);
    playLayout->addStretch();
//    playLayout->setMargin(0);

    QHBoxLayout *rightLayout = new QHBoxLayout(rightWid);
    rightLayout->addWidget(volumeBtn,Qt::AlignRight);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(favBtn,Qt::AlignRight);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(playModeBtn,Qt::AlignRight);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(listBtn,Qt::AlignRight);
    rightLayout->setContentsMargins(0,14,30,14);
//    rightLayout->setMargin(0);

    letfWid->setLayout(leftLayout);
//    letfWid->setStyleSheet("background: red;");
    centreWid->setLayout(playLayout);
//    centreWid->setStyleSheet("background: blue;");
    rightWid->setLayout(rightLayout);
//    rightWid->setStyleSheet("background: green;");

    m_hmainLayout->addWidget(hSlider);
    m_hmainLayout->setMargin(0);
    m_hmainLayout->setSpacing(0);

    m_mainLayout->addWidget(letfWid,0,Qt::AlignLeft);
    m_mainLayout->addWidget(centreWid,0,Qt::AlignCenter);
    m_mainLayout->addWidget(rightWid,0,Qt::AlignRight);
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);

    m_vmainLayout->addLayout(m_hmainLayout);
    m_vmainLayout->addLayout(m_mainLayout);
    this->setLayout(m_vmainLayout);
}

void PlaySongArea::initConnect()
{
//    connect(m_volSliderWid->vSlider,&QSlider::valueChanged,this,&PlaySongArea::slotVolumeChanged);
//    connect(volumeBtn,&QPushButton::clicked,this,&PlaySongArea::slotVolSliderWidget);
//    connect(favBtn,&QPushButton::clicked,this,&PlaySongArea::slotFav);
//    connect(playModeBtn,&QPushButton::clicked,this,&PlaySongArea::slotPlayBackModeChanged);
//    connect(m_playBackModeWid->loopBtn,&QToolButton::clicked,this,&PlaySongArea::slotLoopClicked);
//    connect(m_playBackModeWid->randomBtn,&QToolButton::clicked,this,&PlaySongArea::slotRandomClicked);
//    connect(m_playBackModeWid->sequentialBtn,&QToolButton::clicked,this,&PlaySongArea::slotSequentialClicked);
//    connect(m_playBackModeWid->currentItemInLoopBtn,&QToolButton::clicked,this,&PlaySongArea::slotCurrentItemInLoopClicked);
}

void PlaySongArea::slotVolumeChanged(int values)
{
    if(values == 0)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-muted-symbolic"));
    }
    else if(values > 0 && values <= 50)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-medium-symbolic"));
    }
    else
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-high-symbolic"));
    }
}

void PlaySongArea::slotVolSliderWidget()
{
    if(m_volSliderWid->isVisible())
    {
        m_volSliderWid->hide();
    }
    else
    {
        moveVolSliderWid();
        m_volSliderWid->show();
        m_volSliderWid->raise();
    }
}

void PlaySongArea::slotFav()
{
//    if(favBtn->isVisible())
//    {
////        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
//        favBtn->setIcon(QIcon(":/img/clicked/love1.png"));
//    }
//    else
//    {
//        favBtn->setIcon(QIcon(":/img/default/loveblack2.png"));
//    }

////    favBtn->setIcon(QIcon(":/img/clicked/love1.png"));
}

void PlaySongArea::slotPlayBackModeChanged()
{
    if(m_playBackModeWid->isVisible())
    {
        m_playBackModeWid->hide();
    }
    else
    {
        movePlayModeWid();
        m_playBackModeWid->show();
        m_playBackModeWid->raise();
    }
}

void PlaySongArea::slotLoopClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
    playModeBtn->setToolTip(tr("Loop"));
    m_playBackModeWid->hide();
}

void PlaySongArea::slotRandomClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
    playModeBtn->setToolTip(tr("Random"));
    m_playBackModeWid->hide();
}

void PlaySongArea::slotSequentialClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
    playModeBtn->setToolTip(tr("Sequential"));
    m_playBackModeWid->hide();
}

void PlaySongArea::slotCurrentItemInLoopClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one"));
    playModeBtn->setToolTip(tr("CurrentItemInLoop"));
    m_playBackModeWid->hide();
}

void PlaySongArea::resizeEvent(QResizeEvent *event)
{
    moveVolSliderWid();
    movePlayModeWid();
    QWidget::resizeEvent(event);
}

void PlaySongArea::moveVolSliderWid()
{
    QPoint volumePos = volumeBtn->mapToGlobal(volumeBtn->rect().center());
    m_volSliderWid->adjustSize();
    QSize size = m_volSliderWid->size();
    volumePos.setX(volumePos.x() - size.width() / 2);
    volumePos.setY(volumePos.y() - size.height() - 25);
    QSize volumSize = volumeBtn->size();
    int newPosX = volumePos.x() + size.width() / 2 - volumSize.width() / 2 + 1;
    int newPosY = volumePos.y() + 25 + size.height() - volumSize.height() / 2 + 1;
    m_volSliderWid->changeVolumePos(newPosX, newPosY, volumSize.width(), volumSize.height());
    m_volSliderWid->move(volumePos);
}

void PlaySongArea::movePlayModeWid()
{
    QPoint modePos = playModeBtn->mapToGlobal(playModeBtn->rect().center());
    m_playBackModeWid->adjustSize();
    QSize size = m_playBackModeWid->size();
    modePos.setX(modePos.x() - size.width() / 2);
    modePos.setY(modePos.y() - size.height() - 25);
    QSize modeSize = playModeBtn->size();
    int newPosX = modePos.x() + size.width() / 2 - modeSize.width() / 2 + 1;
    int newPosY = modePos.y() + 25 + size.height() - modeSize.height() / 2 + 1;
    m_playBackModeWid->changePlayModePos(newPosX, newPosY, modeSize.width(), modeSize.height());
    m_playBackModeWid->move(modePos);
}

void PlaySongArea::playcolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        setStyleSheet("background-color:#252526;");

        preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/lastsong.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        favBtn->setStyleSheet("QPushButton{border-image:url(::/img/dark/icon_love2_b@2x.png);}"
                              "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");

        listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/dark/icon_songlist_h@2x.png);}"
                               "QPushButton::hover{border-image:url(:/img/clicked/songlist.png);}"
                               "QPushButton::checked{border-image:url(:/img/clicked/songlist.png);}");

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        playingLabel->setStyleSheet("font-size:14px;color:#F9F9F9;line-height:14px;");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        setStyleSheet("background-color:#FFFFFF;");

        preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/lastsong.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                              "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}"
                              "QPushButton::checked{border-image:url(:/img/clicked/love1.png);}");

        listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/songlist.png)}"
                               "QPushButton::hover{border-image:url(:/img/clicked/songlist.png);}"
                               "QPushButton::checked{border-image:url(:/img/clicked/songlist.png);}");

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        playingLabel->setStyleSheet("line-height:14px;color:#303133;font-size:14px;");

        timeLabel->setStyleSheet("line-height:12px;color:#8F9399;font-size:12px;");
    }
}
