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
