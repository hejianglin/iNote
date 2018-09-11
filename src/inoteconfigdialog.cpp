#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

#include "inoteconfigdialog.h"

iNoteConfigDialog::iNoteConfigDialog(iNoteConfig *config,QWidget *parent)
    : QDialog(parent)
    , m_bNeedUpdateAccessToken(false)
    , m_cConfig(config)
{
    initGui();

}

void iNoteConfigDialog::showEvent(QShowEvent *)
{
    m_edtAppKey->setText(m_cConfig->appKey());
    m_edtSecretKey->setText(m_cConfig->secretKey());

    this->move((QApplication::desktop()->width() - this->width())/2,
               (QApplication::desktop()->height() - this->height())/2);
}

void iNoteConfigDialog::initGui()
{
    //engine
    m_gboxEngine  = new QGroupBox(this);
    m_gboxEngine->setTitle(tr("OCR Engine"));

    m_lblAppKey = new QLabel(this);
    m_lblAppKey->setText("AppKey:");

    m_edtAppKey = new QLineEdit(this);

    m_lblSecretKey = new QLabel(this);
    m_lblSecretKey->setText("SecretKey:");

    m_edtSecretKey = new QLineEdit(this);

    m_layoutEngine = new QGridLayout;
    m_layoutEngine->addWidget(m_lblAppKey,0,0,1,1);
    m_layoutEngine->addWidget(m_edtAppKey,0,1,1,3);
    m_layoutEngine->addWidget(m_lblSecretKey,1,0,1,1);
    m_layoutEngine->addWidget(m_edtSecretKey,1,1,1,3);

    m_gboxEngine->setLayout(m_layoutEngine);

    //tool button
    m_btnOk = new QPushButton(this);
    m_btnOk->setText(tr("Ok"));
    connect(m_btnOk,SIGNAL(clicked()),this,SLOT(slotOk()));

    m_btnCancel = new QPushButton(this);
    m_btnCancel->setText(tr("Cancel"));
    connect(m_btnCancel,SIGNAL(clicked()),this,SLOT(close()));

    m_layoutToolButton = new QHBoxLayout;
    m_layoutToolButton->addStretch();
    m_layoutToolButton->addWidget(m_btnOk);
    m_layoutToolButton->addWidget(m_btnCancel);

    m_layoutMain = new QVBoxLayout;
    m_layoutMain->addWidget(m_gboxEngine);
    m_layoutMain->addStretch();
    m_layoutMain->addLayout(m_layoutToolButton);
    this->setLayout(m_layoutMain);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    resize(500,200);
}

void iNoteConfigDialog::slotOk()
{
    if(m_edtAppKey->text().isEmpty()
            || m_edtSecretKey->text().isEmpty()){
        QMessageBox::critical(this,tr("Error"),tr("Please add the user information of the "
                                                  "engine as required."));
        return ;
    }

    if(m_cConfig->appKey() != m_edtAppKey->text().simplified()
            || m_cConfig->secretKey() != m_edtSecretKey->text().simplified()){
        m_bNeedUpdateAccessToken = true;
        m_cConfig->setAppKey(m_edtAppKey->text().simplified());
        m_cConfig->setSecretKey(m_edtSecretKey->text().simplified());
        QMessageBox::information(this,tr("Info"),tr("The settings are successful, "
                                                    "and user information will be updated later."));
    }
    accept();
}

