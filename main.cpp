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
