#include "tableviewdelegate.h"

TableViewDelegate::TableViewDelegate(QWidget *parent)
    : QStyledItemDelegate(parent),
      m_pOpenButton(new QPushButton()),
      m_pDeleteButton(new QPushButton()),
      m_nSpacing(5),
      m_nWidth(25),
      m_nHeight(20)
{
//    // 设置按钮正常、划过、按下样式
//    m_pOpenButton->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/image/loveblack2.png);} \
//                                 QPushButton:hover {image:url(:/image/play1.png);} \
//                                 QPushButton:pressed {image:url(:/image/pause1.png);}");

//    m_pDeleteButton->setStyleSheet("QPushButton{border:none; background-color:transparent;border-image:url(:/image/pause1.png));} \
//                                 QPushButton:hover{image:url(:/image/play1.png);} \
//                                 QPushButton:pressed{image:url(:/image/loveblack2.png);}");
//    m_list << QStringLiteral("打开") << QStringLiteral("删除");
}

TableViewDelegate::~TableViewDelegate()
{

}

// 绘制按钮
void TableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem  view_option(option);
    if (view_option.state & QStyle::State_HasFocus) {
    view_option.state = view_option.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, view_option, index);
    if(index.row() == hoverrow_)
    {
        painter->fillRect(view_option.rect, QColor("#F0F0F0"));
//        font.setStyle();

    }
    else
    {
        painter->fillRect(view_option.rect,Qt::transparent);
    }
    const QAbstractItemModel *item_model = index.model();
    if (index.column() == 0) {
        QModelIndex index0 = item_model->index(index.row(), 0, QModelIndex());
        QString str0 = item_model->data(index0, Qt::DisplayPropertyRole).toString();
//        str0 = str0.left(10);
        QPalette pat;
        qDebug() <<view_option.textElideMode << view_option.palette;
        painter->drawText(view_option.rect,str0);
    }
    else if(index.column() == 1)
    {
        QModelIndex index1 = item_model->index(index.row(), 1, QModelIndex());
        QString str1 = item_model->data(index1, Qt::DisplayPropertyRole).toString();
        painter->drawText(view_option.rect,str1);
    }
    else if(index.column() == 2)
    {
        QModelIndex index2 = item_model->index(index.row(), 2, QModelIndex());
        QString str2 = item_model->data(index2, Qt::DisplayPropertyRole).toString();
        painter->drawText(view_option.rect,str2);
    }
    else if(index.column() == 3)
    {
        QModelIndex index3 = item_model->index(index.row(), 3, QModelIndex());
        QString str3 = item_model->data(index3, Qt::DisplayPropertyRole).toString();
        painter->drawText(view_option.rect,str3);
    }
}

// 响应按钮事件 - 划过、按下
bool TableViewDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    qDebug()<< "aaaaaaaaaaaaa";
}


void TableViewDelegate::onHoverIndexChanged(const QModelIndex& index)
{
    hoverrow_ = index.row();
//    qDebug() <<"当前悬浮行号" << index.row();
}
