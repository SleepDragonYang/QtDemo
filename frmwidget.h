#ifndef FRMWIDGET_H
#define FRMWIDGET_H

#include <QWidget>
#include "videothread.h"
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QApplication>
#include "iconstyle.h"
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QStandardItemModel>
namespace Ui {
class FrmWidget;
}

class FrmWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrmWidget(QWidget *parent = nullptr);
    ~FrmWidget();
public slots:
    void slotGetOneFrame(QImage img);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void initWindow();

    void on_play();

    void on_openFile();

    void playClick(QModelIndex index);

    void on_maxBtn_clicked();

    void on_volumeBtn_clicked();

    void on_playOrderBtn_clicked();

    void on_singleBtn();

    void on_sequenceBtn();

    void on_listLoopBtn();

    void on_randomBtn();

    void on_singleLoopBtn();

    void on_playList_doubleClicked(const QModelIndex &index);

    void on_listHideBtn_clicked();

private:
    Ui::FrmWidget *ui;
    VideoThread *mPlayer;
    QImage m_Image;
    QStringList fileList;
    QPoint mousePoint;  //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;  //鼠标是否按下
    bool max;           //是否最大化
    QRect location;     //窗口位置大小
    bool mute;
    QActionGroup *group;
    QAction *single    ;
    QAction *sequence  ;
    QAction *listLoop  ;
    QAction *random    ;
    QAction *singleLoop;
    QStandardItemModel *itemModel;
};

#endif // FRMWIDGET_H
