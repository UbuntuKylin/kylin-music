#include "sliderwidget.h"
#include "xatom-helper.h"

SliderWidget::SliderWidget(QWidget *parent) : QWidget(parent)
{
    initUi(); //初始化样式
}

void SliderWidget::initUi()
{
//    vSliderDialog = new QDialog(this);
    setGeometry(813,498,30,90);
    setFixedSize(30,90);
    setStyleSheet("background-color:#FFFFFF;");
//    MotifWmHints hints;
//    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
//    hints.functions = MWM_FUNC_ALL;
//    hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), hints);

    vSlider = new QSlider(this);
    vSlider->installEventFilter(this);
    vSlider->setOrientation(Qt::Vertical);
//    vSlider->setFixedSize(100,12);
    vSlider->setMinimum(0);
    vSlider->setMaximum(100);
    vSlider->setValue(50);
//        vSlider->setSingleStep(1);
    vSlider->setStyleSheet("QSlider::groove:vertical {width:10px;}"
                           "QSlider::add-page:vertical{background:#FFFFFF;}"
                           "QSlider::sub-page:vertical{background:#ECEEF5;}"
                           "QSlider::handle:vertical{width:10px;height:10px;margin-top: 0px;margin-left: -5px;margin-bottom: 0px;margin-right: -5px; }"
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
//                qDebug()<<"pos : "<<pos;
//                qDebug()<<"dur : "<<dur;
//                qDebug()<<"height() : "<<vSlider->height();
//                qDebug()<<"mouseEvent->y() : "<<mouseEvent->y();
            }
        }
    }
    return QObject::eventFilter(obj,event);
}
