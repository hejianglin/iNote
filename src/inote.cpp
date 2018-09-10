#include <QTimer>

#include "inote.h"
#include "inoteconfigdialog.h"

#include "ocrcodec.h"
#include "ocrcodecfactory.h"

iNote::iNote(QWidget *parent)
    : QMainWindow(parent)
{
    //variant
    m_cOCRCodec = OCRCodecFactory::instance();
    connect(m_cOCRCodec,SIGNAL(sigProgress(quint8)),this,SLOT(slotProgress(quint8)));
    connect(m_cOCRCodec,SIGNAL(sigFinished(QString,QString)),
            this,SLOT(slotFinished(QString,QString)));

    //gui
    m_cNoteViewer = new iNoteViewer(this);
    setCentralWidget(m_cNoteViewer);
    setWindowIcon(QIcon("://inote.png"));
}

iNote::~iNote()
{
    OCRCodecFactory::destroy();
}

void iNote::showEvent(QShowEvent *)
{
    //检查 ocr 的配置
    if(m_cConfig.appKey().isEmpty() || m_cConfig.secretKey().isEmpty()){
        slotConfig();
    }else{
        //更新token
        updateAccessToken();

    }
}

void iNote::slotConfig()
{
    iNoteConfigDialog *dialog = new iNoteConfigDialog(&m_cConfig,this);
    dialog->setWindowTitle(tr("Config"));
    dialog->exec();

    if(dialog->isNeddUpdateAccessToken()){
        updateAccessToken();
    }
}

void iNote::generateConfig()
{//to test
}

void iNote::slotProgress(quint8)
{

}

void iNote::slotFinished(const QString &,const QString &)
{

}

void iNote::updateAccessToken()
{
    m_cOCRCodec->setAppKey(m_cConfig.appKey());
    m_cOCRCodec->setSecretKey(m_cConfig.secretKey());
    decode(OCRCodec::RequestType_eAccessToken,"");
}

void iNote::decode(OCRCodec::RequestType type,const QString &file)
{
    m_cOCRCodec->decode(type,file);
    enterBusyMode();
}

void iNote::enterBusyMode()
{

}
