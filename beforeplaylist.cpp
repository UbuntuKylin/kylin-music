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

#include "beforeplaylist.h"
#include <QGraphicsDropShadowEffect>

BeforePlayList::BeforePlayList(QWidget *parent):QWidget(parent)
{
//    setFixedWidth(320);
//    setFixedSize(320,640);
    setGeometry(640,0,320,562);
//    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background:#FFFFFF;");

    initUi();

}

void BeforePlayList::initUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *titleLayout = new QHBoxLayout(this);


    beforePlayList = new QListWidget(this);

    beforePlayList->setContentsMargins(16,0,16,0);
    beforeListTitleLabel = new QLabel(this);
//    beforeListTitleLabel->setText("播放列表");
    beforeListTitleLabel->setText(tr("The playlist"));
    beforeListTitleLabel->setFixedHeight(30);
    beforeListTitleLabel->setStyleSheet("width: 72px;margin-top:2px;\
                                        height: 18px;\
                                        font-size: 18px;\
                                        font-weight: 600;\
                                        color: #303133;\
                                        line-height: 18px;\  ");


    beforeListNumberLabel = new QLabel(this);
    beforeListNumberLabel->setStyleSheet("width: 37px;margin-top:10px;\
                                         height: 14px;\
                                         font-size: 14px;\
                                         font-weight: 400;\
                                         color: #8F9399;\
                                         line-height: 14px;\
                                         ");


    emptyBtn = new QToolButton(this);
    emptyBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    emptyBtn->setIcon(QIcon(":/img/default/delete.png"));
    emptyBtn->setIconSize(QSize(16,16));
//    emptyBtn->setText("清空");
    emptyBtn->setText(tr("empty"));
    emptyBtn->setStyleSheet("QToolButton{font-size: 14px;margin-top:6px;\
                            font-weight: 400;\
                            color: #8F9399;\
                            line-height: 14px;}"
                            "QToolButton::hover{background-color:#FFFFFF;border-radius:16px;}"
                            "QToolButton::pressed{background-color:#FFFFFF;border-radius:16px;}"
                            );

    QWidget *titleWidget = new QWidget(this);
    titleWidget->setFixedHeight(45);
    titleWidget->setStyleSheet("background-color:#FFFFFF;");

    titleLayout->addWidget(beforeListTitleLabel,0,Qt::AlignLeft);
    titleLayout->addWidget(beforeListNumberLabel,Qt::AlignLeft);
    titleLayout->addStretch();
    titleLayout->addWidget(emptyBtn,0,Qt::AlignRight);
    titleWidget->setLayout(titleLayout);


    mainLayout->addWidget(titleWidget,0,Qt::AlignTop);
    mainLayout->addWidget(beforePlayList);
    mainLayout->setSpacing(0);
//    mainLayout->setMargin(0);

//    QWidget *mainWidget = new QWidget(this);
//    mainWidget->setLayout(mainLayout);

//    beforePlayList->setStyleSheet("QScrollBar{background-color:#F0F0F0;border-radius:3px;width:6px;}"
//                                  "QScrollBar::handle:vertical{background-color:#C2C2C2;border-radius:3px;width:6px;min-height:20px;}"
//                                  "QListWidget{background-color:#FFFFFF;border:0px;}"
//                                  "QListWidget::item{height:40px;}"
//                                  "QListWidget::item:selected{background-color:#FFFFFF;color:#FF4848;}"
//                                  "QListWidget::item:hover{background-color:#FFFFFF;}"
//                                  );

    beforePlayList->setStyleSheet("QListWidget{background-color:#FFFFFF;border:0px;}"
                                  "QListWidget::item{height:40px;}"
                                  "QListWidget::item:selected{background-color:#FFFFFF;color:#FF4848;}"
                                  "QListWidget::item:hover{background-color:#FFFFFF;}"
                                  );


//    beforePlayList->setStyleSheet("QScrollBar{background-color:white;border-radius:2px;width:4px;}"
//                                  "QScrollBar::handle:vertical{background-color:#8F9399;border-radius:2px;width:4px;}"
//                                  "QScrollBar::handle:vertical:hover{background-color:#303032;border-radius:4px;width:4px;min-height:20;}"
//                                  );


    this->setLayout(mainLayout);
//    mainWidget->raise();
    this->raise();
}
