#include "mainwid.h"
#include "connection.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QApplication a(argc, argv);
    if(!CreatConnection())
    {
        return 1;
    }
    a.setWindowIcon(QIcon(":/img/kylin-music.png"));
    MainWid w;
    w.show();
    return a.exec();
}
