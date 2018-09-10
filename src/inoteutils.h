#ifndef INOTEUTILS_H
#define INOTEUTILS_H

#include <QUrl>
#include <QList>

class iNoteUtils
{
public:
    iNoteUtils();

    static void delay(int msec);

    static bool isValidUrls(QList<QUrl> urls);
};

#endif // INOTEUTILS_H
