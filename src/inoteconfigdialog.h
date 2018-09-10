#ifndef INOTECONFIGDIALOG_H
#define INOTECONFIGDIALOG_H

#include <QDialog>

#include "inoteconfig.h"

class iNoteConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit iNoteConfigDialog(iNoteConfig *,QWidget *parent = 0);

    bool isNeddUpdateAccessToken() { return false; }

signals:

public slots:


private:
    void initGui();
    void setMainLayoyt();
};

#endif // INOTECONFIGDIALOG_H
