#include "widget.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString locale = QLocale::system().name();
    QTranslator trans_global, trans_menu;
    if(locale == "zh_CN"){
        trans_global.load(":/translations/kylin-music_side.qm");
        trans_menu.load(":/translations/qt_zh_CN.qm");
        a.installTranslator(&trans_global);
        a.installTranslator(&trans_menu);
    }
#ifndef QT_NO_TRANSLATION
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator();
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translator);
    else
        qDebug() << "Failed to load Chinese translation file.";
#endif
    Widget w;
    w.show();
    qDebug() << "错误状态：啊啊啊啊啊啊啊啊啊啊";
    return a.exec();
}
