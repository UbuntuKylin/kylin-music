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

#include "allpupwindow.h"
#include "widgetstyle.h"


#include <QDebug>
#include <QMessageBox>


AllPupWindow::AllPupWindow(QWidget *parent) : QWidget(parent)
{


    inpupdialog();
    this->setStyleSheet("border-radius:6px;");
    dlgcolor();

}

void AllPupWindow::closeDialog()
{
    pupDialog->close();

    enterLineEdit->clear();
}

void AllPupWindow::inpupdialog()
{

    pupDialog = new QDialog(this);
    pupDialog->resize(424,172);
    pupDialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


    QVBoxLayout *testLayout = new QVBoxLayout(pupDialog);
    QHBoxLayout *titleLayout = new QHBoxLayout(pupDialog);
    titleLab = new QLabel(pupDialog);
    closeBtn = new QPushButton(pupDialog);


//    titleLab->setText("提示信息");
    titleLab->setText(tr("Prompt information"));

    titleLayout->addWidget(titleLab,Qt::AlignLeft);
    titleLayout->addWidget(closeBtn,0,Qt::AlignRight);



    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);

    enterLineEdit = new QLineEdit(pupDialog);


    confirmBtn = new QPushButton(pupDialog);

    cancelBtn = new QPushButton(pupDialog);

    QHBoxLayout *btnLayout = new QHBoxLayout(pupDialog);

//    confirmBtn->setText("确认");
    confirmBtn->setText(tr("confirm"));
    confirmBtn->setFixedSize(64,30);
//    cancelBtn->setText("取消");
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->setFixedSize(64,30);

    btnLayout->addWidget(cancelBtn, 0, Qt::AlignRight);
    btnLayout->addWidget(confirmBtn, Qt::AlignRight);
    btnLayout->setSpacing(20);
    btnLayout->setMargin(0);

//    enterLineEdit->setPlaceholderText("请输入歌单标题：");
    enterLineEdit->setPlaceholderText(tr("Please enter the title of the playlist:"));

    testLayout->addLayout(titleLayout);
    testLayout->addWidget(enterLineEdit);
    testLayout->addLayout(btnLayout);

    testLayout->setMargin(0);
    testLayout->setSpacing(20);

    pupDialog->setLayout(testLayout);
    pupDialog->setContentsMargins(10, 0, 10, 0);

    // 设置边框阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(6);
    cancelBtn->setGraphicsEffect(shadowEffect);

//    connect(addSongListBtn, SIGNAL(clicked(bool)), this, SLOT(addSongList()));
    connect(closeBtn,SIGNAL(clicked(bool)),this,SLOT(closeDialog()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(closeDialog()));
}

void AllPupWindow::dlgcolor()
{
    if(WidgetStyle::themeColor == 1 )
    {
        cancelBtn->setStyleSheet("border-radius:4px;border:1px solid #DDDFE7;color:#606265;");

        enterLineEdit->setStyleSheet("width:392px;height:32px;border-radius:4px;\
                                     border: 1px solid #606265;color:#F9F9F9;");

        pupDialog->setStyleSheet("background-color:#3D3D41;"
                                     "border-radius:6px;");

        confirmBtn->setStyleSheet("background-color:#FF4848;border-radius:4px;color:#FFFFFF;");

        titleLab->setStyleSheet("width:80px;height:14px;font-size: 14px;\
                                    font-weight: 600;\
                                    border:none;\
                                    color: #F9F9F9;\
                                    line-height:14px;");

        closeBtn->setStyleSheet("QPushButton{width:30px;height:30px;background:transparent;border-radius:4px;;\
                                   border-image:url(:/img/lightshow/close.png);}\
                                   QPushButton::hover{border-image:url(:/img/hover/close.png);\
                                                      background:#FD9595;opacity:0.04}\
                                   QPushButton::pressed{border-image:url(:/img/clicked/close.png);background:#ED6464;}");


    }

    else if(WidgetStyle::themeColor == 0)
    {
        cancelBtn->setStyleSheet("border-radius:4px;border:1px solid #DDDFE7;color:#8F9399;");

        enterLineEdit->setStyleSheet("width:392px;height:32px;border-radius:4px;border:1px solid #DDDFE7;");

        pupDialog->setStyleSheet("background-color:#FFFFFF;"
                                 "border-radius:6px;");
        confirmBtn->setStyleSheet("background-color:#FF4848;border-radius:4px;color:#FFFFFF;");

        titleLab->setStyleSheet("width:80px;height:14px;font-size: 14px;\
                                font-weight: 600;\
                                border:none;\
                                color: #1B1B1B;\
                                line-height:14px;");
        closeBtn->setStyleSheet("QPushButton{width:30px;height:30px;background:transparent;border-radius:4px;;\
                               border-image:url(:/img/default/close.png);}\
                               QPushButton::hover{border-image:url(:/img/hover/close.png);\
                                                  background:#FD9595;opacity:0.04}\
                               QPushButton::pressed{border-image:url(:/img/clicked/close.png);background:#ED6464;}");
    }

}




