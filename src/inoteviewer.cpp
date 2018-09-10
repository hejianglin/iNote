#include "inoteviewer.h"
#include "inoteutils.h"
#include <QPixmap>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

#include <QKeyEvent>
#include <QMouseEvent>

#include <QApplication>
#include <QClipboard>

iNoteViewer::iNoteViewer(QWidget *parent) : QWidget(parent)
{
    initGui();

    setAcceptDrops(true);
}

void iNoteViewer::initGui()
{
    m_lblImage = new QLabel(this);
    m_lblImage->setAlignment(Qt::AlignCenter);
    m_lblImage->setText(tr("image place"));

    m_tedtResult = new QTextEdit(this);

    m_tbtnDisplayImage = new QToolButton(this);
    m_tbtnDisplayImage->setText(tr("Display Image"));

    m_tbtnCopy = new QToolButton(this);
    m_tbtnCopy->setText(tr("Copy"));

    m_tbtnDisplayOnTop = new QToolButton(this);
    m_tbtnDisplayOnTop->setText(tr("Top"));

    m_layoutToolButton = new QHBoxLayout;
    m_layoutToolButton->setContentsMargins(0,0,0,0);
    m_layoutToolButton->addWidget(m_tbtnDisplayImage);
    m_layoutToolButton->addStretch();
    m_layoutToolButton->addWidget(m_tbtnCopy);
    m_layoutToolButton->addStretch();
    m_layoutToolButton->addWidget(m_tbtnDisplayOnTop);

    m_layoutResult = new QVBoxLayout;
    m_layoutResult->setContentsMargins(0,0,0,0);

    m_layoutResult->addWidget(m_tedtResult);
    m_layoutResult->addLayout(m_layoutToolButton);

    m_frmResult = new QFrame(this);
    m_frmResult->setLayout(m_layoutResult);

    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(Qt::Vertical);

    m_splitter->insertWidget(0,m_lblImage);
    m_splitter->insertWidget(1,m_frmResult);

//    setMainLayout();

    m_layoutMain = new QHBoxLayout(this);
    m_layoutMain->addWidget(m_splitter);
    this->setLayout(m_layoutMain);
}


void iNoteViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()){
        QList<QUrl> urls = event->mimeData()->urls();
        if (iNoteUtils::isValidUrls(urls)){
            event->acceptProposedAction();
        }
        else{
            event->ignore();
        }
    } else {
        event->ignore();
    }
}


void iNoteViewer::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void iNoteViewer::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()){
        slotPasteLayer(event->mimeData());
        event->accept();
    }
    else {
        event->ignore();
    }
}

void iNoteViewer::keyPressEvent(QKeyEvent *pEvent)
{
    if (pEvent->modifiers() == Qt::ControlModifier && pEvent->key() == Qt::Key_V){
        slotPasteLayer(nullptr);
        return;
    }
}



void iNoteViewer::slotPasteLayer(const QMimeData* mimeData)
{
    if (!mimeData){
        mimeData = QApplication::clipboard()->mimeData();
    }
    QList<QUrl> urls = mimeData->urls();
    if (urls.size() > 0)
    {
        //LoadImageFile(urls[0].toLocalFile());
        m_lblImage->setPixmap(QPixmap(urls[0].toLocalFile()));
    }
    else if (mimeData->hasImage())
    {
        //save to temp
//        QImage stImage = qvariant_cast<QImage>(mimeData->imageData());
//        DecodeImage(stImage);
    }
    else
    {
        return;
    }


}
