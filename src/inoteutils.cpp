#include <QTime>
#include <QCoreApplication>

#include "inoteutils.h"

iNoteUtils::iNoteUtils()
{

}

void iNoteUtils::delay(int mesc)
{
    QTime dieTime = QTime::currentTime().addMSecs(mesc);
    while (QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


bool iNoteUtils::isValidUrls(QList<QUrl> urls)
{
    QString suffix1(".png"), suffix2(".bmp"), suffix3(".jpg"), suffix4(".jpeg");
    bool ok = true;
    for (QUrl url : urls)
    {
        QString filename = url.toLocalFile();
        if (filename.endsWith(suffix1, Qt::CaseInsensitive) ||
            filename.endsWith(suffix2, Qt::CaseInsensitive) ||
            filename.endsWith(suffix3, Qt::CaseInsensitive) ||
            filename.endsWith(suffix4, Qt::CaseInsensitive))
        {
        }
        else
        {
            ok = false;
            break;
        }
    }
    return ok;
}
