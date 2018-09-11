#ifndef INOTECONFIGDIALOG_H
#define INOTECONFIGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>


#include "inoteconfig.h"

class iNoteConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit iNoteConfigDialog(iNoteConfig *,QWidget *parent = 0);

    bool isNeddUpdateAccessToken() { return m_bNeedUpdateAccessToken; }

protected:
    void showEvent(QShowEvent *) override;

private slots:
    void slotOk();

private:
    //data
    bool m_bNeedUpdateAccessToken;

    iNoteConfig *m_cConfig;

    //gui
    QGroupBox *m_gboxEngine;
    QLabel *m_lblAppKey;
    QLineEdit *m_edtAppKey;
    QLabel *m_lblSecretKey;
    QLineEdit *m_edtSecretKey;
    QGridLayout *m_layoutEngine;

    QPushButton *m_btnOk;
    QPushButton *m_btnCancel;
    QHBoxLayout *m_layoutToolButton;

    QVBoxLayout *m_layoutMain;


    //function
    void initGui();
    void setMainLayoyt();
};

#endif // INOTECONFIGDIALOG_H
