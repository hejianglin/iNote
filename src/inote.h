#ifndef INOTE_H
#define INOTE_H

#include <QMainWindow>

#include "inoteconfig.h"

class iNote : public QMainWindow
{
    Q_OBJECT

public:
    iNote(QWidget *parent = 0);
    ~iNote();


private:
    iNoteConfig m_cConfig;



    void generateConfig();
};

#endif // INOTE_H
