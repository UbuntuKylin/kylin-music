#include "draglistwidget.h"

DragListWidget::DragListWidget(QWidget *parent):QListWidget(parent)
{
    setAcceptDrops(true);
}

DragListWidget::~DragListWidget()
{

}

//bool DragListWidget::eventFilter(QObject *obj, QEvent *eve)
//{
//    if(obj == )
//}

//void DragListWidget::mousePressEvent(QMouseEvent *event)
//{
//    //确保左键拖拽.
//    if (event->button() == Qt::LeftButton)
//    {
//        qDebug()<<"mousePressEvent mousePressEvent mousePressEvent";
//        //先保存拖拽的起点.
//        dragPoint = event->pos();
//        //保留被拖拽的项.
//        dragItem = this->itemAt(event->pos());
//    }
//    //保留原QListWidget部件的鼠标点击操作.
//    QListWidget::mousePressEvent(event);
//}

//void DragListWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    //确保按住左键移动.
//    if (event->buttons() & Qt::LeftButton)
//    {
//        QPoint temp = event->pos() - dragPoint;
//        //只有这个长度大于默认的距离,才会被系统认为是形成拖拽的操作.
//        if (temp.manhattanLength() > QApplication::startDragDistance())
//        {
//            QDrag *drag = new QDrag(this);
//            QMimeData *mimeData = new QMimeData;
//            mimeData->setText(dragItem->text());
//            drag->setMimeData(mimeData);
//            auto action = drag->exec(Qt::CopyAction | Qt::MoveAction);

//            if (action == (Qt::CopyAction) || (action == Qt::MoveAction))
//            {
//                //当成功拖拽后，删除拖拽项.
//                auto i = this->takeItem(this->row(dragItem));
//                delete i;
//            }
//        }
//    }
//    QListWidget::mouseMoveEvent(event);
//}

//void DragListWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    QListWidget::mouseReleaseEvent(event);
//}

void DragListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    // 判断拖拽文件类型，文件名 接收该动作
    if(!event->mimeData()->hasFormat("text/uri-list"))
    {
        event->setDropAction(Qt::CopyAction);
        event->acceptProposedAction();
    }
    QListWidget::dragEnterEvent(event);
}

void DragListWidget::dragMoveEvent(QDragMoveEvent *event)
{
//    event->setDropAction(Qt::MoveAction);
}

void DragListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
//    QListWidget::dragLeaveEvent(event);
}

void DragListWidget::dropEvent(QDropEvent *event)
{
    // 判断拖拽文件类型，文件名 接收该动作
    if(!event->mimeData()->hasFormat("text/uri-list"))
    {
        return ;
    }

    auto urls = event->mimeData()->urls();
    qDebug()<<"urls urls urls urls urls urls urls:"<<urls;
    QStringList localpath;
    for(auto &url : urls)
    {
        localpath << url.toLocalFile();
    }

    if(!localpath.isEmpty())
    {

    }
}
