#ifndef INOTE_H
#define INOTE_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QQueue>
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
  //  void slotCheckAccessToken();
    void slotConfig();
    void slotProgress(quint8);
    void slotFinished(const QString &,const QString &);

private:
    //Data
    iNoteConfig m_cConfig;

    iNoteViewer *m_cNoteViewer;

    OCRCodec *m_cOCRCodec;

    //Function
    void decode(OCRCodec::RequestType,const QString &);
    void generateConfig();

    void updateAccessToken();
    void enterBusyMode();
    void quitBusyMode();
};

#endif // INOTE_H
