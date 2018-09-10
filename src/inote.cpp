#include "inote.h"
#include "ocrcodec_baidu.h"

//截图并显示到画布,自动转换

iNote::iNote(QWidget *parent)
    : QMainWindow(parent)
{
    setAcceptDrops(true);

    //this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
}

iNote::~iNote()
{

}

void iNote::generateConfig()
{//to test
}
