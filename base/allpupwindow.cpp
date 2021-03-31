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
//#include "xatom-helper.h"

#include <QDebug>
#include <QMessageBox>


AllPupWindow::AllPupWindow(QWidget *parent) : QWidget(parent)
{
    inpupdialog();
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
//    pupDialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//    MotifWmHints hints;
//    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
//    hints.functions = MWM_FUNC_ALL;
//    hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(pupDialog->winId(), hints);
    pupDialog->setWindowModality(Qt::ApplicationModal); //弹出自定义对话框时主界面不可操作
//    pupDialog->setModal(true);
    testLayout = new QVBoxLayout();
    titleLayout = new QHBoxLayout();
    titleLab = new QLabel(this);
    closeBtn = new QToolButton(this);
    closeBtn->setFixedSize(30,30);
    closeBtn->setIcon(QIcon::fromTheme("window-close-symbolic"));
    closeBtn->setProperty("isWindowButton", 0x2);
    closeBtn->setProperty("useIconHighlightEffect", 0x8);
    closeBtn->setAutoRaise(true);

//    titleLab->setText("提示信息");
    titleLab->setText(tr("Prompt information"));

    titleLayout->addWidget(titleLab,Qt::AlignLeft);
    titleLayout->addWidget(closeBtn,0,Qt::AlignRight);

    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);

    enterLineEdit = new QLineEdit(pupDialog);

    confirmBtn = new QPushButton(pupDialog);

    cancelBtn = new QPushButton(pupDialog);

    btnLayout = new QHBoxLayout();

    stackWid =new QStackedWidget();
    stackWid->addWidget(enterLineEdit);

    stackWid->setFixedSize(392,32);
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
    testLayout->addWidget(stackWid);
    testLayout->addLayout(btnLayout);

    testLayout->setMargin(0);
    testLayout->setSpacing(20);

    pupDialog->setLayout(testLayout);
    pupDialog->setContentsMargins(10, 0, 10, 0);

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

        pupDialog->setStyleSheet("background-color:#3D3D41;");

        confirmBtn->setStyleSheet("QPushButton{background:#3790FA;border-radius:4px;color:#FFFFFF;}"
                                  "QPushButton::hover{background:#40A9FB;}"
                                  "QPushButton::pressed{background:#296CD9;}"
                                  );

        titleLab->setStyleSheet("width:80px;height:14px;\
                                font-weight: 600;\
                                border:none;\
                                color: #F9F9F9;\
                                line-height:14px;");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        cancelBtn->setStyleSheet("QPushButton{border-radius:4px;border:1px solid #DDDFE7;color:#8F9399;}"
                                 "QPushButton::hover{background:#F2F6FD;}");

        enterLineEdit->setStyleSheet("width:392px;height:32px;border-radius:4px;border:1px solid #DDDFE7;");

        pupDialog->setStyleSheet("background-color:#FFFFFF;");

        confirmBtn->setStyleSheet("QPushButton{background:#3790FA;border-radius:4px;color:#FFFFFF;}"
                                  "QPushButton::hover{background:#40A9FB;}"
                                  "QPushButton::pressed{background:#296CD9;}"
                                  );

        titleLab->setStyleSheet("width:80px;height:14px;\
                                font-weight: 600;\
                                border:none;\
                                color: #1B1B1B;\
                                line-height:14px;");
    }
}

