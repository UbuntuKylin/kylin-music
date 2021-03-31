#include "playbackmodewidget.h"
#include "base/widgetstyle.h"

PlayBackModeWidget::PlayBackModeWidget(QWidget *parent) : QDialog(parent)
{
    installEventFilter(this);
    setAttribute(Qt::WA_StyledBackground,true);
    initWidget();
    playModecolor();

}

void PlayBackModeWidget::initWidget()
{
    setFixedSize(118,150);
    QVBoxLayout *vLayout = new QVBoxLayout;

    QHBoxLayout *loopLayout = new QHBoxLayout();
    loopBtn = new QToolButton(this);
    loopBtn->setFixedSize(114, 30);
    loopBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    loopBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
    loopBtn->setIconSize(QSize(16,16));
    loopBtn->setText(tr("Loop"));
    loopLayout->addWidget(loopBtn);
    loopLayout->setMargin(0);

    QHBoxLayout *randomLayout = new QHBoxLayout();
    randomBtn = new QToolButton(this);
    randomBtn->setFixedSize(114, 30);
    randomBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    randomBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
    randomBtn->setIconSize(QSize(16,16));
    randomBtn->setText(tr("Random"));
    randomLayout->addWidget(randomBtn);
    randomLayout->setMargin(0);

    QHBoxLayout *sequentialLayout = new QHBoxLayout();
    sequentialBtn = new QToolButton(this);
    sequentialBtn->setFixedSize(114, 30);
    sequentialBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sequentialBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
    sequentialBtn->setIconSize(QSize(16,16));
    sequentialBtn->setText(tr("Sequential"));
    sequentialLayout->addWidget(sequentialBtn);
    sequentialLayout->setMargin(0);

    QHBoxLayout *currentItemInLoopLayout = new QHBoxLayout();
    currentItemInLoopBtn = new QToolButton(this);
    currentItemInLoopBtn->setFixedSize(114, 30);
    currentItemInLoopBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    currentItemInLoopBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one"));
    currentItemInLoopBtn->setIconSize(QSize(16,16));
    currentItemInLoopBtn->setText(tr("CurrentItemInLoop"));
    currentItemInLoopLayout->addWidget(currentItemInLoopBtn);
    currentItemInLoopLayout->setMargin(0);

    vLayout->addLayout(loopLayout,Qt::AlignTop);
    vLayout->addLayout(randomLayout,Qt::AlignVCenter);
    vLayout->addLayout(sequentialLayout,Qt::AlignVCenter);
    vLayout->addLayout(currentItemInLoopLayout,Qt::AlignBottom);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    this->setLayout(vLayout);
}

bool PlayBackModeWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);
    if(eventType != "xcb_generic_event_t")
    {
        return false;
    }

    xcb_generic_event_t *event = (xcb_generic_event_t*)message;
    switch (event->response_type & ~0x80)
    {
        case XCB_FOCUS_OUT:
            QRect rect(playModePosX, playModePosY, playModePosWidth, playModePosHeight);
            if(rect.contains(QCursor::pos(), false))
            {
                return 0;
            }
            else
            {
                this->hide();
                break;
            }
    }
    return false;
}

void PlayBackModeWidget::changePlayModePos(int posX, int posY, int width, int height)
{
    playModePosX = posX;
    playModePosY = posY;
    playModePosWidth = width;
    playModePosHeight = height;
}

void PlayBackModeWidget::playModecolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        setStyleSheet("background-color:#252526");
        loopBtn->setStyleSheet("QToolButton{padding-left:7px;background-color:#1F2022;color:#F9F9F9;border-radius:4px;}"
                               "QToolButton::hover{background-color:#303032;border-radius:4px;}");
        randomBtn->setStyleSheet("QToolButton{padding-left:7px;background-color:#1F2022;color:#F9F9F9;border-radius:4px;}"
                                 "QToolButton::hover{background-color:#303032;border-radius:4px;}");
        sequentialBtn->setStyleSheet("QToolButton{padding-left:7px;background-color:#1F2022;color:#F9F9F9;border-radius:4px;}"
                                     "QToolButton::hover{background-color:#303032;border-radius:4px;}");
        currentItemInLoopBtn->setStyleSheet("QToolButton{padding-left:7px;background-color:#1F2022;color:#F9F9F9;border-radius:4px;}"
                                            "QToolButton::hover{background-color:#303032;border-radius:4px;}");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        setStyleSheet("background-color:#FFFFFF");
        loopBtn->setStyleSheet("QToolButton{padding-left:7px;\
                               background-color:#FAFAFA;color:#303133;border-radius:4px;}"
                               "QToolButton::hover{background-color:#EEEEEE;border-radius:4px;}");
        randomBtn->setStyleSheet("QToolButton{padding-left:7px;\
                                 background-color:#FAFAFA;color:#303133;border-radius:4px;}"
                                 "QToolButton::hover{background-color:#EEEEEE;border-radius:4px;}");
        sequentialBtn->setStyleSheet("QToolButton{padding-left:7px;\
                                     background-color:#FAFAFA;color:#303133;border-radius:4px;}"
                                     "QToolButton::hover{background-color:#EEEEEE;border-radius:4px;}");
        currentItemInLoopBtn->setStyleSheet("QToolButton{padding-left:7px;\
                                            background-color:#FAFAFA;color:#303133;border-radius:4px;}"
                                            "QToolButton::hover{background-color:#EEEEEE;border-radius:4px;}");
    }
}




