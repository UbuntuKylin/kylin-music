#ifndef KYLINMUISC_H
#define KYLINMUISC_H

#include <QObject>
#include <QPoint>
#include <QString>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QThread>

#include <mainwid.h>

class MusicListWid;

class KylinMuisc : public QObject
{
    Q_OBJECT
public:
    enum ExitCode { ErrorArgument = -3, NoAction = -2, NoRunningInstance = -1, NoError = 0, NoExit = 1 };

    KylinMuisc(const QString &arch = QString::null, const QString &snap = QString::null, QObject * parent = 0);
    ~KylinMuisc();

    ExitCode processArgs(QStringList args);

//    MainWid *gui();
signals:

private:

//    static MainWid *main_window;
    void deleteGUI();
    void showInfo();
    void deleteConfig();
    QStringList m_filesToPlay;
    QString m_subtitleFile;
    QString m_mediaTitle; //!< Force a title for the first file
    // Change position and size
    bool m_moveGui;
    QPoint m_guiPosition;
    bool m_resizeGui;
    QSize m_guiSize;
    QString m_arch;
    QString m_snap;
    QThread *m_thread = nullptr;
    MusicListWid *wid;
public:
    QString Dir;
    QString musicPath;

    QFileInfo fileInfo;
    QByteArray byteArray;
    QString musicName;
    QString musicSinger;
    QString musicAlbum;
    QString musicTime;
    QString musicSize;
    QString musicType;
};

#endif // KYLINMUISC_H
