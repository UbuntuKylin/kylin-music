#include "sliderwidget.h"
#include "base/widgetstyle.h"
#include "xatom-helper.h"

SliderWidget::SliderWidget(QWidget *parent) : QDialog(parent)
{
    installEventFilter(this);
//    this->setAttribute(Qt::WA_DeleteOnClose);
    initUi(); //初始化样式
    volSliderColor();
}

void SliderWidget::initUi()
{
    setFixedSize(30,90);
    vSlider = new QSlider(this);
    vSlider->installEventFilter(this);
    vSlider->setOrientation(Qt::Vertical);
    vSlider->setMinimum(0);
    vSlider->setMaximum(100);
    vSlider->setValue(50);
    vSlider->setStyleSheet("QSlider::groove:vertical{background:transparent;height: 70px;width: 4px;background:#3790FA;border-radius:5px;}"
                           "QSlider::add-page:vertical{background:#3790FA;}"
                           "QSlider::sub-page:vertical{background:#ECEEF5;}"
                           "QSlider::handle:vertical{width: 4px;margin: -5 0 -5 0;border-image:url(:/img/default/point.png);}"
                           );

    HLayout = new QHBoxLayout;
    HLayout->addWidget(vSlider);
    this->setLayout(HLayout);
}

bool SliderWidget::eventFilter(QObject *obj, QEvent *event)   //鼠标滑块点击
{
   if(obj == vSlider)
    {
        if (event->type()==QEvent::MouseButtonPress)           //判断类型
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) //判断左键
            {
               int dur = vSlider->maximum() - vSlider->minimum();
               int pos = vSlider->minimum() + (dur * (vSlider->height() - ((double)mouseEvent->y())) / vSlider->height());
               if(pos != vSlider->sliderPosition())
                {
                    vSlider->setValue(pos);
                }
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

bool SliderWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
            QRect rect(volunmPosX, volunmPosY, volunmPosWidth, volunmPosHeight);
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

void SliderWidget::changeVolumePos(int posX, int posY, int width, int height)
{
    volunmPosX = posX;
    volunmPosY = posY;
    volunmPosWidth = width;
    volunmPosHeight = height;
}

void SliderWidget::volSliderColor()
{
    if(WidgetStyle::themeColor == 1)
    {
        setStyleSheet("background-color:#252526;");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        setStyleSheet("background-color:#FFFFFF;");
    }
}
