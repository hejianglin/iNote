#ifndef INOTEVIEWER_H
#define INOTEVIEWER_H

#include <QWidget>
#include <QSplitter>
#include <QLabel>
#include <QTextEdit>
#include <QFrame>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMimeData>


class iNoteViewer : public QWidget
{
    Q_OBJECT
public:
    explicit iNoteViewer(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *pEvent) Q_DECL_OVERRIDE;

signals:
    void sigAddTask();

public slots:
    void slotPasteLayer(const QMimeData* mimeData);


private:

    QLabel *m_lblImage;

    QFrame *m_frmResult;
    QTextEdit *m_tedtResult;
    QToolButton *m_tbtnDisplayImage;
    QToolButton *m_tbtnCopy;
    QToolButton *m_tbtnDisplayOnTop;
    QHBoxLayout *m_layoutToolButton;
    QVBoxLayout *m_layoutResult;

    QSplitter *m_splitter;
    QHBoxLayout *m_layoutMain;


    void initGui();
    void initVariant();
    void setMainLayout();
};

#endif // INOTEVIEWER_H
