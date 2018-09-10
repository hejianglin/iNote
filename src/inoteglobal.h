#ifndef INOTEGLOBAL_H
#define INOTEGLOBAL_H


#if 1
#include <QDebug>
#define DEBUG(x) do {\
    qDebug()<<__FILE__<<"["<<__LINE__<<"]"<<x;\
    }while(0);
#else
#endif

#endif // INOTEGLOBAL_H
