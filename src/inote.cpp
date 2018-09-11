#include <QMenu>
#include <QTimer>
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

#include "inote.h"
#include "inoteconfigdialog.h"
#include "ocrcodecfactory.h"

iNote::iNote(QWidget *parent)
    : QMainWindow(parent)
{
    //variant
    m_cOCRCodec = OCRCodecFactory::instance();
    connect(m_cOCRCodec,SIGNAL(sigProgress(quint8)),this,SLOT(slotProgress(quint8)));
    connect(m_cOCRCodec,SIGNAL(sigFinished(QString,QString)),
            this,SLOT(slotFinished(QString,QString)));
    
    //main windows
    m_cNoteViewer = new iNoteViewer(this);
    connect(m_cNoteViewer,SIGNAL(sigAddTask(OCRCodec::RequestType,QString))
            ,this,SLOT(decode(OCRCodec::RequestType,QString)));
    setCentralWidget(m_cNoteViewer);
    setWindowIcon(QIcon("://inote_connected.png"));
    
    //system tray
    QAction *actAbout = new QAction(this);
    actAbout->setText(tr("About"));
    connect(actAbout,SIGNAL(triggered()),this,SLOT(slotAbout()));
    
    QAction *actConfig = new QAction(this);
    actConfig->setText(tr("Option"));
    connect(actConfig,SIGNAL(triggered()),this,SLOT(slotConfig()));
    
    QAction *quitConfig = new QAction(this);
    quitConfig->setText(tr("Quit"));
    connect(quitConfig,SIGNAL(triggered()),this,SLOT(close()));
    
    QMenu *trayMenu = new QMenu(this);
    trayMenu->addAction(actAbout);
    trayMenu->addAction(actConfig);
    trayMenu->addSeparator();
    trayMenu->addAction(quitConfig);
    
    m_systemTray = new QSystemTrayIcon(QIcon("://inote_disconnected.png"),this);
    m_systemTray->setToolTip(tr("iNote"));
    m_systemTray->setContextMenu(trayMenu);
    m_systemTray->show();
    
    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(slotTrayActived(QSystemTrayIcon::ActivationReason)));

    //run
    QTimer::singleShot(500,this,SLOT(slotCheckAccessToken()));
}

iNote::~iNote()
{
    OCRCodecFactory::destroy();
}

void iNote::showEvent(QShowEvent *)
{
    static bool bRunFirst = true;
    if(bRunFirst){
        bRunFirst  = false;
        this->move((QApplication::desktop()->width() - this->width())/2,
                   (QApplication::desktop()->height() - this->height())/2);
    }
}

void iNote::slotCheckAccessToken()
{
    //检查 ocr 的配置
    if(m_cConfig.appKey().isEmpty() || m_cConfig.secretKey().isEmpty()){
        QMessageBox::warning(this,tr("Error"),tr("The OCR engine is empty. "
                                                 "Please set up the engine first."));
        slotConfig();
    }else{
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

void iNote::slotProgress(quint8 progress)
{
    m_systemTray->showMessage(tr("Progress"),tr("Completed %1").arg(progress));
}

void iNote::slotFinished(const QString &ret,const QString &error)
{
    if(error.isEmpty() && !ret.isEmpty()){
        if(m_cOCRCodec->decodeType() != OCRCodec::RequestType_eAccessToken){
            m_cNoteViewer->updateResultText(ret);
        }else{
            m_systemTray->setIcon(QIcon("://inote_connected.png"));
            m_cOCRCodec->setAccessToken(ret);
        }
        m_systemTray->showMessage(tr("ok"),tr("success"));
    }else
    {
        m_systemTray->showMessage(tr("error"),tr("fail:%1").arg(error));
    }

    quitBusyMode();
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

void iNote::quitBusyMode()
{

}

void iNote::slotAbout()
{
    
}

void iNote::slotTrayActived(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        this->show();
    }
}
