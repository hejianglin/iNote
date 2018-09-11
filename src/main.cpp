#include "inote.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    iNote w;
    w.show();

    return a.exec();
}
