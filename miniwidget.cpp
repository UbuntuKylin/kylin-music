#include "miniwidget.h"

miniWidget::miniWidget(QWidget *parent) : QFrame(parent)
{
    setFixedSize(300,60);
    this->setObjectName("miniWidget");

    m_mouseState = false;
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明
//    setStyleSheet("#miniWidget{border-top-left-radius:12px;border-bottom-left-radius:12px;}");


    setMouseTracking(true);
    init_miniWidget();

//    this->setLayout(m_hLayout);
    this->setLayout(m_HMainLayout);

    // background-color:#FFFFFF;
//    setStyleSheet("#miniWidget{border-radius:12px;background-color:transparent;}");
}

void miniWidget::mousePressEvent(QMouseEvent *event)
{
    m_WindowPos = this->pos();
    if(QRect(0,0,width(),height()).contains(event->pos()) && event->button() == Qt::LeftButton)
    {
        m_MousePos = event->globalPos();
        m_mouseState = true;
    }
}

void miniWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseState)
    {
        move(m_WindowPos + (event->globalPos() - m_MousePos));
    }
}

void miniWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseState = false;
}

void miniWidget::enterEvent(QEvent *)
{
    m_songNameLab->setVisible(false);
    m_timeLab->setVisible(false);
    m_palyFrame->setVisible(true);
    coverWid->setVisible(true);
    this->setFixedWidth(328);
}

void miniWidget::leaveEvent(QEvent *)
{
    m_songNameLab->setVisible(true);
    m_timeLab->setVisible(true);
    m_palyFrame->setVisible(false);
    coverWid->setVisible(false);
    this->setFixedWidth(300);

}

void miniWidget::init_miniWidget()
{
//    setStyleSheet(::miniMainColor);
    setStyleSheet("border-radius:12px;background-color:#FFFFFF;");

    m_HMainLayout = new QHBoxLayout(this);
    m_hLayout = new QHBoxLayout(this);

    m_mainFrame = new QFrame;
    m_mainFrame->setFixedSize(300,60);
    m_mainFrame->setStyleSheet("border-radius:6px;background-color:#FFFFFF");


    m_hMainLayout = new QHBoxLayout(m_mainFrame);

    /*******************************************/
    m_coverLabel = new QLabel;
    m_coverLabel->setFixedSize(42,42);
    m_coverLabel->setCursor(Qt::PointingHandCursor);
    m_coverLabel->setStyleSheet("QLabel{border-image:url(:/img/kylin-music.png);}");

    /************************************/
    m_vInfoLayout = new QVBoxLayout;

    m_songNameLab = new QLabel;
    m_songNameLab->setFixedSize(200,18);
    m_songNameLab->setText("爸爸妈妈");
    m_songNameLab->setAlignment(Qt::AlignCenter);
    m_songNameLab->setStyleSheet("QLabel{color:#303133;}");

    m_timeLab = new QLabel;
    m_timeLab->setFixedSize(200,18);
    m_timeLab->setText("04:21");
    m_timeLab->setAlignment(Qt::AlignCenter);
    m_timeLab->setStyleSheet("QLabel{color:#8F9399;}");

    m_vInfoLayout->setMargin(3);
    m_vInfoLayout->setSpacing(3);
    m_vInfoLayout->addWidget(m_songNameLab);
    m_vInfoLayout->addWidget(m_timeLab);

    /******************************************/
    m_palyFrame = new QFrame;
    m_palyFrame->setFixedWidth(120);
    m_palyFrame->setVisible(false);

    m_hPlayLayout = new QHBoxLayout(m_palyFrame);

    m_preBtn = new QPushButton;
    m_preBtn->setFixedSize(16,16);
    m_preBtn->setCursor(Qt::PointingHandCursor);
    m_preBtn->setIcon(QIcon(":/img/default/lastsong.png"));
//    m_preBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/lastsong.png);}");

    m_playStateBtn = new QPushButton;
    m_playStateBtn->setFixedSize(36,36);
    m_playStateBtn->setCursor(Qt::PointingHandCursor);
    m_playStateBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/play2.png);}");

    m_nextBtn = new QPushButton;
    m_nextBtn->setFixedSize(16,16);
    m_nextBtn->setCursor(Qt::PointingHandCursor);
    m_nextBtn->setIcon(QIcon(":/img/default/nextsong_b@2x.png"));
//    m_nextBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/nextsong_b@2x.png);}");

    m_hPlayLayout->setMargin(0);
    m_hPlayLayout->setSpacing(9);
    m_hPlayLayout->addWidget(m_preBtn);
    m_hPlayLayout->addWidget(m_playStateBtn);
    m_hPlayLayout->addWidget(m_nextBtn);
    /****************************************************/

    m_loveBtn = new QPushButton;
    m_loveBtn->setFixedSize(16,16);
    m_loveBtn->setCursor(Qt::PointingHandCursor);
    m_loveBtn->setIcon(QIcon(":/img/default/loveblack2.png"));
//    m_loveBtn->setStyleSheet("QPushButton{background:transparent;\
//                             border-image:url(:/img/default/loveback2.png);}");


    m_orderBtn = new QPushButton;
    m_orderBtn->setFixedSize(16,16);
    m_orderBtn->setCursor(Qt::PointingHandCursor);
    m_orderBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/sequence.png);}"
                              "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}");

    /***************************************/
    m_vSysLayout = new QVBoxLayout;
    coverWid = new QWidget(this);
//    coverWid->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明
    coverWid->setStyleSheet("background-color:rgba(0,0,0,0.4);\
                            border-top-right-radius:6px;border-bottom-right-radius:6px;\
                            border-top-left-radius:0px;border-bottom-left-radius:0px");

    m_closeBtn = new QPushButton;
    m_closeBtn->setFixedSize(30,30);
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    m_closeBtn->setIcon(QIcon(":/img/clicked/close.png"));
    m_closeBtn->setIconSize(QSize(30, 30));
    m_closeBtn->setStyleSheet("QPushButton{background:transparent;}");
    connect(m_closeBtn,&QPushButton::clicked,this,&miniWidget::hide);
    //just hide not close

    m_recoveryWinBtn = new QPushButton;
    m_recoveryWinBtn->setFixedSize(30,30);
    m_recoveryWinBtn->setCursor(Qt::PointingHandCursor);
    m_recoveryWinBtn->setIcon(QIcon(":/img/lightshow/maximize.png"));
    m_recoveryWinBtn->setIconSize(QSize(30, 30));
    m_recoveryWinBtn->setStyleSheet("QPushButton{background-color:transparent;}");

    m_vSysLayout->setSpacing(0);
    m_vSysLayout->setMargin(0);
    m_vSysLayout->addWidget(m_closeBtn);
    m_vSysLayout->addWidget(m_recoveryWinBtn);
//    m_vSysLayout->addStretch();

    coverWid->setLayout(m_vSysLayout);
    /********************************************/

    m_hMainLayout->setMargin(10);
    m_hMainLayout->addWidget(m_coverLabel);
    m_hMainLayout->addLayout(m_vInfoLayout);
    m_hMainLayout->addStretch();
    m_hMainLayout->addWidget(m_palyFrame);
    m_hMainLayout->addStretch();
    m_hMainLayout->addWidget(m_loveBtn);
    m_hMainLayout->addWidget(m_orderBtn);
//    m_hMainLayout->addLayout(m_vSysLayout);


    m_mainFrame->setLayout(m_hMainLayout);

    m_HMainLayout->setMargin(0);
    m_HMainLayout->addWidget(m_mainFrame);
    m_HMainLayout->addWidget(coverWid);

//    QFrame *controlFrame = new QFrame(this);
//    controlFrame->setLayout(m_vSysLayout);

//    m_hLayout->addWidget(m_mainFrame);
//    m_hLayout->addWidget(controlFrame);

//    m_mainFrame->setStyleSheet("border-radius:12px;");
}
