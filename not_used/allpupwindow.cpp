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
#include "xatom-helper.h"

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
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(pupDialog->winId(), hints);
    pupDialog->setWindowModality(Qt::ApplicationModal); //弹出自定义对话框时主界面不可操作
//    pupDialog->setModal(true);
    testLayout = new QVBoxLayout(pupDialog);
    titleLayout = new QHBoxLayout(pupDialog);
    titleLab = new QLabel(pupDialog);
    closeBtn = new QToolButton(pupDialog);
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

    QLabel *promptLabel = new QLabel(this);       //您确定要删除歌单吗提示信息
//    promptLabel->setText(tr("您确定要删除歌单吗?"));
    promptLabel->setText(tr("Are you sure you want to delete the playlist?"));
    QLabel *promptRemoveLabel = new QLabel(this);    //默认歌单无法删除提示信息
//    promptRemoveLabel->setText(tr("默认歌单无法删除!"));
    promptRemoveLabel->setText(tr("The default playlist cannot be deleted!"));
    QLabel *promptExistLabel = new QLabel(this);    //歌单已存在提示信息
//    promptExistLabel->setText(tr("歌单名已存在!"));
    promptExistLabel->setText(tr("The single name already exists!"));
    QLabel *promptRenameLabel = new QLabel(this);         //歌单无法重命名提示信息
//    promptRenameLabel->setText(tr("默认歌单无法重命名!"));
    promptRenameLabel->setText(tr("The default playlist cannot be renamed!"));

    confirmBtn = new QPushButton(pupDialog);

    cancelBtn = new QPushButton(pupDialog);

    btnLayout = new QHBoxLayout(pupDialog);

    stackWid =new QStackedWidget();
    stackWid->addWidget(enterLineEdit);
    stackWid->addWidget(promptLabel);
    stackWid->addWidget(promptRemoveLabel);
    stackWid->addWidget(promptExistLabel);
    stackWid->addWidget(promptRenameLabel);
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

        titleLab->setStyleSheet("width:80px;height:14px;font-size: 14px;\
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

        titleLab->setStyleSheet("width:80px;height:14px;font-size: 14px;\
                                font-weight: 600;\
                                border:none;\
                                color: #1B1B1B;\
                                line-height:14px;");
    }

}


SongInfoWidget::SongInfoWidget(QWidget *parent) : QWidget(parent)
{
    inSongInfoDlg();
    songInfoDlgColor();
}

void SongInfoWidget::inSongInfoDlg()
{
    songInfoDlg = new QDialog(this);
    songInfoDlg->setFixedSize(424,327);
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(songInfoDlg->winId(), hints);
    songInfoDlg->setWindowModality(Qt::ApplicationModal);  //弹出自定义对话框时主界面不可操作
    songInfoDlg->move(QApplication::desktop()->screen()->rect().center() - songInfoDlg->rect().center());


//    QWidget *infotitleWid = new QWidget(songInfoDlg);

    QWidget *titleWid = new QWidget(songInfoDlg);
    QWidget *artistWid = new QWidget(songInfoDlg);
    QWidget *albumWid = new QWidget(songInfoDlg);
    QWidget *typeWid = new QWidget(songInfoDlg);
    QWidget *sizeWid = new QWidget(songInfoDlg);
    QWidget *timeWid = new QWidget(songInfoDlg);
    QWidget *pathWid = new QWidget(songInfoDlg);

//    QWidget *btnWid = new QWidget(songInfoDlg);

    QVBoxLayout *infotestLayout = new QVBoxLayout(songInfoDlg);//总体的纵向布局

    songInfoTitleLab = new QLabel(songInfoDlg);
    songInfoCloseBtn = new QToolButton(songInfoDlg);
    songInfoCloseBtn->setFixedSize(30,30);
    songInfoCloseBtn->setIcon(QIcon::fromTheme("window-close-symbolic"));
    songInfoCloseBtn->setProperty("isWindowButton", 0x2);
    songInfoCloseBtn->setProperty("useIconHighlightEffect", 0x8);
    songInfoCloseBtn->setAutoRaise(true);

    titleLab  = new QLabel(songInfoDlg);
    artistLab = new QLabel(songInfoDlg) ;
    albumLab  = new QLabel(songInfoDlg);
    typeLab   = new QLabel(songInfoDlg);
    sizeLab   = new QLabel(songInfoDlg);
    timeLab   = new QLabel(songInfoDlg);
    pathLab   = new QLabel(songInfoDlg);


    musicNameLab = new QLabel(songInfoDlg);
    singerNameLab = new QLabel(songInfoDlg);
    albumNameLab = new QLabel(songInfoDlg);
//    musicNameEdit = new QLineEdit(songInfoDlg);
//    singerNameEdit = new QLineEdit(songInfoDlg);
//    albumNameEdit = new QLineEdit(songInfoDlg);
    fileTypeLab = new QLabel(songInfoDlg);
    fileSizeLab = new QLabel(songInfoDlg);
    fileTimeLab = new QLabel(songInfoDlg);
    filePathLab = new QLabel(songInfoDlg);

//    fileTypeWid = new QWidget(songInfoDlg);
//    fileSizeWid = new QWidget(songInfoDlg);
//    fileTimeWid = new QWidget(songInfoDlg);
//    filePathWid = new QWidget(songInfoDlg);


//    titleLab->setText("歌曲名称:");
//    artistLab->setText("歌手:");
//    albumLab->setText("专辑:");
//    typeLab->setText("文件类型:");
//    sizeLab->setText("文件大小:");
//    timeLab->setText("文件时长:");
//    pathLab->setText("文件位置:");

//    titleLab->setText("提示信息");
//    songInfoTitleLab->setText(tr("Prompt information"));


//    songInfoEdit->setStyleSheet("border-width:0;border-style:outset;border: none;");
//    song

    songInfoConfirmBtn = new QPushButton(songInfoDlg);

    songInfoCancelBtn = new QPushButton(songInfoDlg);

    QHBoxLayout *infotitleLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *titleLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *artistLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *albumLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *typeLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *sizeLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *timeLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *pathLabLayout = new QHBoxLayout(songInfoDlg);
    QHBoxLayout *btnLayout = new QHBoxLayout(songInfoDlg);


//    songInfoTitleLab->setText(tr("歌曲信息"));
    songInfoTitleLab->setText(tr("Song information"));

    infotitleLayout->addWidget(songInfoTitleLab,Qt::AlignLeft);
    infotitleLayout->addWidget(songInfoCloseBtn,0,Qt::AlignRight);
//    infotitleWid->setLayout(infotitleLayout);
//    infotitleLayout->setMargin(2);
    infotitleLayout->setSpacing(0);
    infotitleLayout->setContentsMargins(10,8,0,0);
//    confirmBtn->setText("确认");
//    songInfoConfirmBtn->setText(tr("confirm"));
//    songInfoConfirmBtn->setText(tr("确认"));
    songInfoConfirmBtn->setText(tr("confirm"));
    songInfoConfirmBtn->setFixedSize(64,30);
//    cancelBtn->setText("取消");
    songInfoCancelBtn->setText(tr("cancel"));
    songInfoCancelBtn->setFixedSize(64,30);
    songInfoCancelBtn->hide();

    btnLayout->addWidget(songInfoCancelBtn, 0, Qt::AlignRight);
    btnLayout->addWidget(songInfoConfirmBtn,0, Qt::AlignRight);
    btnLayout->setContentsMargins(0,0,10,14);

    titleLabLayout->addWidget(titleLab, 0, Qt::AlignRight);
    titleLabLayout->addWidget(musicNameLab,0,Qt::AlignLeft);
    titleWid->setLayout(titleLabLayout);
    titleLabLayout->setContentsMargins(10,0,16,0);

    artistLabLayout->addWidget(artistLab, 0, Qt::AlignRight);
    artistLabLayout->addWidget(singerNameLab,0,Qt::AlignLeft);
    artistWid->setLayout(artistLabLayout);
    artistLabLayout->setContentsMargins(10,0,16,0);

    albumLabLayout->addWidget(albumLab, 0, Qt::AlignRight);
    albumLabLayout->addWidget(albumNameLab,0,Qt::AlignLeft);
    albumWid->setLayout(albumLabLayout);
    albumLabLayout->setContentsMargins(10,0,16,0);

    typeLabLayout->addWidget(typeLab,0,Qt::AlignLeft);
    typeLabLayout->addWidget(fileTypeLab,0,Qt::AlignLeft);
    typeWid->setLayout(typeLabLayout);
    typeLabLayout->setContentsMargins(10,0,16,0);

    sizeLabLayout->addWidget(sizeLab,0,Qt::AlignLeft);
    sizeLabLayout->addWidget(fileSizeLab,0,Qt::AlignLeft);
    sizeWid->setLayout(sizeLabLayout);
    sizeLabLayout->setContentsMargins(10,0,16,0);

    timeLabLayout->addWidget(timeLab,Qt::AlignLeft);
    timeLabLayout->addWidget(fileTimeLab,0,Qt::AlignLeft);
    timeWid->setLayout(timeLabLayout);
    timeLabLayout->setContentsMargins(10,0,16,0);

    pathLabLayout->addWidget(pathLab,Qt::AlignLeft);
    pathLabLayout->addWidget(filePathLab,0,Qt::AlignLeft);
    pathWid->setLayout(pathLabLayout);
    pathLabLayout->setContentsMargins(10,0,16,0);

//    QWidget
//    enterLineEdit->setPlaceholderText("请输入歌单标题：");
//    musicNameEdit->setPlaceholderText(tr("Please enter the title of the playlist:"));

//    infotestLayout->addWidget(infotitleWid);
//    infotestLayout->addWidget(musicNameEdit);
    infotestLayout->addLayout(infotitleLayout);

    infotestLayout->addWidget(titleWid,0,Qt::AlignLeft);
    infotestLayout->addWidget(artistWid,0,Qt::AlignLeft);
    infotestLayout->addWidget(albumWid,0,Qt::AlignLeft);
    infotestLayout->addWidget(typeWid,0,Qt::AlignLeft);
    infotestLayout->addWidget(sizeWid,0,Qt::AlignLeft);
    infotestLayout->addWidget(timeWid,0,Qt::AlignLeft);
    infotestLayout->addWidget(pathWid,0,Qt::AlignLeft);

    infotestLayout->addLayout(btnLayout);


    infotestLayout->setMargin(0);
    infotestLayout->setSpacing(5);

    songInfoDlg->setLayout(infotestLayout);
    songInfoDlg->setContentsMargins(5, 0, 5, 0);
    songInfoDlg->show();

//    QVBoxLayout *mainInfoLayout = new QVBoxLayout(this);
//    mainInfoLayout->addWidget(songInfoDlg);
//    this->setLayout(mainInfoLayout);
//    this->show();

//    // 设置边框阴影
//    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setOffset(0, 0);
//    shadowEffect->setColor(Qt::gray);
//    shadowEffect->setBlurRadius(6);
//    cancelBtn->setGraphicsEffect(shadowEffect);

//    connect(addSongListBtn, SIGNAL(clicked(bool)), this, SLOT(addSongList()));
    connect(songInfoCloseBtn,SIGNAL(clicked(bool)),this,SLOT(songInfoCloseDialog()));
    connect(songInfoConfirmBtn,SIGNAL(clicked(bool)),this,SLOT(songInfoCloseDialog()));
}

void SongInfoWidget::songInfoCloseDialog()
{
    songInfoDlg->close();
}

void SongInfoWidget::songInfoDlgColor()
{
    if(WidgetStyle::themeColor == 1 )    //深色模式
    {
        songInfoCancelBtn->setStyleSheet("border-radius:4px;border:1px solid #DDDFE7;color:#606265;");

        titleLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                 font-weight:400;\
                                 border:none;\
                                 color: #F9F9F9;\
                                 line-height:14px;");
        artistLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                  font-weight:400;\
                                  border:none;\
                                  color: #F9F9F9;\
                                  line-height:14px;");
        albumLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                 font-weight:400;\
                                 border:none;\
                                 color: #F9F9F9;\
                                 line-height:14px;");
        typeLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #F9F9F9;\
                                line-height:14px;");
        sizeLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #F9F9F9;\
                                line-height:14px;");
        timeLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #F9F9F9;\
                                line-height:14px;");
        pathLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #F9F9F9;\
                                line-height:14px;");

//        musicNameEdit->setStyleSheet("width:326px;height:16px;\
//                                      border:none;\
//                                      font-weight:400;\
//                                      line-height:14px;\
//                                      font-size:14px;\
//                                      color:#F9F9F9;");
//        singerNameEdit->setStyleSheet("width:326px;height:16px;\
//                                      border:none;\
//                                      font-weight:400;\
//                                      line-height:14px;\
//                                      font-size:14px;\
//                                      color:#F9F9F9;");
//        albumNameEdit->setStyleSheet("width:326px;height:16px;\
//                                      border:none;\
//                                      font-weight:400;\
//                                      line-height:14px;\
//                                      font-size:14px;\
//                                      color:#F9F9F9;");
        musicNameLab->setStyleSheet("width:326px;height:14px;\
                                    border:none;\
                                    font-weight:400;\
                                    line-height:14px;\
                                    font-size:14px;\
                                    color:#F9F9F9;");
        singerNameLab->setStyleSheet("width:326px;height:14px;\
                                     border:none;\
                                     font-weight:400;\
                                     line-height:14px;\
                                     font-size:14px;\
                                     color:#F9F9F9;");
        albumNameLab->setStyleSheet("width:326px;height:14px;\
                                     border:none;\
                                     font-weight:400;\
                                     line-height:14px;\
                                     font-size:14px;\
                                     color:#F9F9F9;");
        fileTypeLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#F9F9F9;");
        fileSizeLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#F9F9F9;");
        fileTimeLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#F9F9F9;");
        filePathLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#F9F9F9;");

        songInfoDlg->setStyleSheet("background-color:#3D3D41;");

        songInfoConfirmBtn->setStyleSheet("QPushButton{background:#3790FA;border-radius:4px;color:#FFFFFF;}"
                                          "QPushButton::hover{background:#40A9FB;}"
                                          "QPushButton::pressed{background:#296CD9;}"
                                          );


        songInfoTitleLab->setStyleSheet("width:80px;height:14px;font-size: 14px;\
                                         font-weight: 600;\
                                         border:none;\
                                         color:#F9F9F9;\
                                         line-height:14px;");

        songInfoCloseBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;}\
                                         QToolButton::hover{background:#F86457;opacity:0.04}\
                                         QToolButton::pressed{background:#E44C50;}");

    }

    else if(WidgetStyle::themeColor == 0)    //浅色模式   歌曲信息
    {
        songInfoCancelBtn->setStyleSheet("border-radius:4px;border:1px solid #DDDFE7;color:#8F9399;");

        titleLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                 font-weight:400;\
                                 border:none;\
                                 color: #606265;\
                                 line-height:14px;");
        artistLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                  font-weight:400;\
                                  border:none;\
                                  color: #606265;\
                                  line-height:14px;");
        albumLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                 font-weight:400;\
                                 border:none;\
                                 color: #606265;\
                                 line-height:14px;");
        typeLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #606265;\
                                line-height:14px;");
        sizeLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #606265;\
                                line-height:14px;");
        timeLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #606265;\
                                line-height:14px;");
        pathLab->setStyleSheet("width:60px;height:14px;font-size:14px;\
                                font-weight:400;\
                                border:none;\
                                color: #606265;\
                                line-height:14px;");

//        musicNameEdit->setStyleSheet("width:326px;height:16px;\
//                                      border:none;\
//                                      font-weight:400;\
//                                      line-height:14px;\
//                                      font-size:14px;\
//                                      color:#606265;");
//        singerNameEdit->setStyleSheet("width:326px;height:16px;\
//                                      border:none;\
//                                      font-weight:400;\
//                                      line-height:14px;\
//                                      font-size:14px;\
//                                      color:#606265;");
//        albumNameEdit->setStyleSheet("width:326px;height:16px;\
//                                      border:none;\
//                                      font-weight:400;\
//                                      line-height:14px;\
//                                      font-size:14px;\
//                                      color:#606265;");
        musicNameLab->setStyleSheet("width:326px;height:14px;\
                                    border:none;\
                                    font-weight:400;\
                                    line-height:14px;\
                                    font-size:14px;\
                                    color:#606265;");
        singerNameLab->setStyleSheet("width:326px;height:14px;\
                                     border:none;\
                                     font-weight:400;\
                                     line-height:14px;\
                                     font-size:14px;\
                                     color:#606265;");
        albumNameLab->setStyleSheet("width:326px;height:14px;\
                                     border:none;\
                                     font-weight:400;\
                                     line-height:14px;\
                                     font-size:14px;\
                                     color:#606265;");
        fileTypeLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#606265;");
        fileSizeLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#606265;");
        fileTimeLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#606265;");
        filePathLab->setStyleSheet("width:326px;height:14px;\
                                   border:none;\
                                   font-weight:400;\
                                   line-height:14px;\
                                   font-size:14px;\
                                   color:#606265;");
        songInfoDlg->setStyleSheet("background-color:#FFFFFF;");
//                                   "border-radius:6px;");
        songInfoConfirmBtn->setStyleSheet("QPushButton{background:#3790FA;border-radius:4px;color:#FFFFFF;}"
                                          "QPushButton::hover{background:#40A9FB;}"
                                          "QPushButton::pressed{background:#296CD9;}"
                                          );

        songInfoTitleLab->setStyleSheet("width:80px;height:14px;font-size: 14px;\
                                         font-weight: 600;\
                                         border:none;\
                                         color:#1B1B1B;\
                                         line-height:14px;");
//        songInfoCloseBtn->setStyleSheet("QPushButton{width:30px;height:30px;background:transparent;border-radius:4px;\
//                                         border-image:url(:/img/default/close.png);}\
//                                         QPushButton::hover{border-image:url(:/img/hover/close.png);\
//                                         background:#FD9595;opacity:0.04}\
//                                         QPushButton::pressed{border-image:url(:/img/clicked/close.png);background:#ED6464;}");
        songInfoCloseBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;}\
                                         QToolButton::hover{background:#F86457;opacity:0.04}\
                                         QToolButton::pressed{background:#E44C50;}");

    }
}
