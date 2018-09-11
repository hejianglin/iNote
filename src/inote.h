#ifndef INOTE_H
#define INOTE_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "ocrcodec.h"
#include "inoteviewer.h"
#include "inoteconfig.h"

class iNote : public QMainWindow
{
    Q_OBJECT

public:
    iNote(QWidget *parent = 0);
    ~iNote();

protected:
    void showEvent(QShowEvent *) override;

private slots:
    void slotAbout();
    void slotConfig();
    void slotProgress(quint8);
    void slotFinished(const QString &,const QString &);
    void slotTrayActived(QSystemTrayIcon::ActivationReason reason);
    void slotCheckAccessToken();

    void decode(OCRCodec::RequestType,const QString &);

private:
    //Data
    iNoteConfig m_cConfig;
    OCRCodec *m_cOCRCodec;

    //view
    iNoteViewer *m_cNoteViewer;
    QSystemTrayIcon *m_systemTray;

    //Function
    void updateAccessToken();
    void enterBusyMode();
    void quitBusyMode();
};

#endif // INOTE_H
