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

#include <QApplication>
#include <QTime>
#include <QTranslator>
#include <QLibraryInfo>
#include <QStringList>
#include <QStandardPaths>
#include <fcntl.h>
#include <syslog.h>
#include <QTextCodec>

#include "mainwid.h"
#include "connection.h"
#include "xatom-helper.h"
#include "myapplication.h"
#include "kylinmuisc.h"

void msgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    static FILE *fp = NULL; // 使用静态变量，进行数据持久保持
    static char logPath[255] = {0};
    static int uid = -1;

    Q_UNUSED(context);

    // 初始执行时，设置log文件路径
    if (uid == -1) {
        uid = getuid();
        sprintf(logPath, "/tmp/%s.log", "kylin-music");
    }

    if (access(logPath, F_OK|W_OK) == 0) {
        // log文件存在且可写
        if (fp == NULL) {
            // log文件未被打开过
            fp = fopen(logPath, "a+");
        }
    } else {
        // log文件不存在或者不可写，则需要判断是否被打开过，被打开过就需要关闭
        if (fp != NULL)
            fclose(fp);
        fp = NULL;
    }

    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeStr = currentTime.toString("yy.MM.dd hh:mm:ss +zzz");

    // 获取用于控制命令行输出的环境变量
    char *ctrlEnv = getenv("XXXX_DEBUG");
    QString env;

    // 格式化输出字符串，添加消息发生时间、消息等级
    QString outMsg;
    switch (type) {
    case QtDebugMsg:
        outMsg = QString("[%1 D]: %2").arg(timeStr).arg(msg);
        break;
    case QtInfoMsg:
        outMsg = QString("[%1 I]: %2").arg(timeStr).arg(msg);
        break;
    case QtWarningMsg:
        outMsg = QString("[%1 W]: %2").arg(timeStr).arg(msg);
        break;
    case QtCriticalMsg:
        outMsg = QString("[%1 C]: %2").arg(timeStr).arg(msg);
        break;
    case QtFatalMsg:
        outMsg = QString("[%1 F]: %2").arg(timeStr).arg(msg);
    }

    if (fp != NULL) {
        // 日志文件存在，则输出到日志文件中
        fprintf(fp, "%s\n", outMsg.toUtf8().data());
        fflush(fp);
    }

    if (ctrlEnv != NULL) {
        // 环境变量为true或者1，则将信息输出到命令行
        env = QString(ctrlEnv).toLower();
        if ((env == "true") || (env == "1")) {
            fprintf(stdout, "%s\n", outMsg.toStdString().c_str());
            fflush(stdout);
        }
    }

    // 遇到致命错误，需要终止程序（这里终止程序是可选的）
    if (type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[])
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MyApplication a("kylin-music", argc, argv );
    QStringList args = a.arguments();
    qDebug()<<"argc : "<<argc;
    qDebug()<<"argv : "<<argv;
    qDebug()<<"args : "<<args;
//    QApplication a(argc, argv);
//    if(!CreatConnection())
//    {
//        return 1;
//    }
    a.setWindowIcon(QIcon(":/img/kylin-music.png"));

    // 国际化
    QString locale = QLocale::system().name();
    QTranslator trans_global, trans_menu;
    if(locale == "zh_CN"){
        trans_global.load(":/translations/kylin-music_zh_CN.qm");
        trans_menu.load(":/translations/qt_zh_CN.qm");
        a.installTranslator(&trans_global);
        a.installTranslator(&trans_menu);
    }
    qDebug() << "=================argc is " <<argc << argv[0] << argv[1];

    QString str = "";
    if (argc > 1)
    {
        str = argv[1];
    }
    MainWid w(str);

    // 添加窗管协议
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(w.winId(), hints);

//    w.processArgs(args);
    w.show();

    return a.exec();
}
