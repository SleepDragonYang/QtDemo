#include "frmwidget.h"
#include "ui_frmwidget.h"
#include <QDebug>
#include <QAction>
#include <QSize>
FrmWidget::FrmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmWidget),
    mousePressed(false),
    max(false),
    mute(false)
{
    ui->setupUi(this);
    initWindow();
}

FrmWidget::~FrmWidget()
{
    delete ui;
}

void FrmWidget::slotGetOneFrame(QImage img)
{
    ui->page_video_start->setImage(img);
    update();
}

void FrmWidget::initWindow()
{
    this->setWindowTitle(tr("卧龙影音"));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(402, 227);
    location = this->geometry();
    ui->label_title->installEventFilter(this);
    ui->page_video_start->installEventFilter(this);
    ui->widget_home->installEventFilter(this);
    IconStyle::Instance()->setIcon(ui->minBtn, QChar(0xf068), 17);
    IconStyle::Instance()->setIcon(ui->maxBtn, QChar(0xf2d0), 17);
    IconStyle::Instance()->setIcon(ui->closeBtn, QChar(0xf00d), 17);
    IconStyle::Instance()->setIcon(ui->openFileBtn, QChar(0xf114), 17);
    IconStyle::Instance()->setIcon(ui->openFileBtn_2, QChar(0xf114), 17);
    IconStyle::Instance()->setIcon(ui->playBtn, QChar(0xf04b), 30);
    IconStyle::Instance()->setIcon(ui->playBtn_2, QChar(0xf04b), 30);
    IconStyle::Instance()->setIcon(ui->backwardBtn, QChar(0xf04a), 15);
    IconStyle::Instance()->setIcon(ui->forwardBtn, QChar(0xf04e), 15);
    IconStyle::Instance()->setIcon(ui->stopBtn, QChar(0xf04d), 15);
    IconStyle::Instance()->setIcon(ui->previousBtn, QChar(0xf048), 15);
    IconStyle::Instance()->setIcon(ui->nextBtn, QChar(0xf051), 15);
    IconStyle::Instance()->setIcon(ui->volumeBtn, QChar(0xf027), 17);
    IconStyle::Instance()->setIcon(ui->playOrderBtn, QChar(0xf178), 17);
    IconStyle::Instance()->setIcon(ui->listHideBtn, QChar(0xf105), 17);
    connect(ui->minBtn, &QPushButton::clicked, this, &FrmWidget::showMinimized);
    connect(ui->closeBtn, &QPushButton::clicked, this, &FrmWidget::close);
    connect(ui->openFileBtn, &QPushButton::clicked, this, &FrmWidget::on_openFile);
    connect(ui->openFileBtn_2, &QPushButton::clicked, this, &FrmWidget::on_openFile);
    connect(ui->openFileTbtn, &QPushButton::clicked, this, &FrmWidget::on_openFile);
    connect(ui->playBtn, &QPushButton::clicked, this, &FrmWidget::on_play);

    single    = new QAction(this);
    sequence   = new QAction(this);
    listLoop  = new QAction(this);
    random     = new QAction(this);
    singleLoop = new QAction(this);
    single->setText(tr("单个播放"));
    sequence->setText(tr("顺序播放"));
    listLoop->setText(tr("列表循环"));
    random->setText(tr("随机播放"));
    singleLoop->setText(tr("单个循环"));
    single->setCheckable(true);
    sequence->setCheckable(true);
    listLoop->setCheckable(true);
    random->setCheckable(true);
    singleLoop->setCheckable(true);
    single->setChecked(true);
    group = new QActionGroup(this);
    group->addAction(single);
    group->addAction(sequence);
    group->addAction(listLoop);
    group->addAction(random);
    group->addAction(singleLoop);
    group->setExclusive(true);

    itemModel = new QStandardItemModel(this);
    ui->playList->setModel(itemModel);
    mPlayer = new VideoThread;
    connect(mPlayer, &VideoThread::sig_GetOneFrame, this, &FrmWidget::slotGetOneFrame);
}
bool FrmWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_maxBtn_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
void FrmWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() & Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmWidget::on_play()
{
    mPlayer->quit();
    int count = ui->playList->model()->rowCount();
    if(count <= 0)
        return;
    int curRow = ui->playList->currentIndex().row();
    mPlayer->setFilePath(fileList.at(curRow));
    mPlayer->start();
    ui->stackedWidget->setCurrentWidget(ui->page_start);
    ui->stackedWidget_video->setCurrentWidget(ui->page_video_start);
}

void FrmWidget::on_openFile()
{
    fileList = QFileDialog::getOpenFileNames(this, "打开一个或多个", "/home", "媒体文件 (*.mp4 *.mkv *.avi *.rmvb *.3gp *.mov *.flv *.swf *.mp3)");

    int nCount = fileList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(fileList.at(i));
        QStringList tempList = string.split("/");
        QStandardItem *item = new QStandardItem(tempList.last());
        itemModel->appendRow(item);
    }
    ui->playList->setModel(itemModel);

}
void FrmWidget::playClick(QModelIndex index)
{
    QString fileName = index.data().toString();
    ui->label_title->setText(fileName);
    on_play();

}
void FrmWidget::on_maxBtn_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconStyle::Instance()->setIcon(ui->maxBtn, QChar(0xf2d0), 17);
        ui->maxBtn->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconStyle::Instance()->setIcon(ui->maxBtn, QChar(0xf2d2), 17);
        ui->maxBtn->setToolTip("还原");
    }
    max = !max;
}

void FrmWidget::on_volumeBtn_clicked()
{
    if(mute)
    {
        IconStyle::Instance()->setIcon(ui->volumeBtn, QChar(0xf026), 17);
        ui->volumeBtn->setToolTip("取消静音");
    }
    else
    {
        IconStyle::Instance()->setIcon(ui->volumeBtn, QChar(0xf027), 17);
        ui->volumeBtn->setToolTip("静音");
    }
    mute = !mute;
}

void FrmWidget::on_playOrderBtn_clicked()
{
    //定义一个右键弹出菜单
    QMenu popMenu;
    popMenu.addAction(single);
    popMenu.addAction(sequence);
    popMenu.addAction(listLoop);
    popMenu.addAction(random);
    popMenu.addAction(singleLoop);
    QObject::connect(single, &QAction::triggered, this, &FrmWidget::on_singleBtn);
    QObject::connect(sequence, &QAction::triggered, this, &FrmWidget::on_sequenceBtn);
    QObject::connect(listLoop,&QAction::triggered, this, &FrmWidget::on_listLoopBtn);
    QObject::connect(random, &QAction::triggered,this, &FrmWidget::on_randomBtn);
    QObject::connect(singleLoop, &QAction::triggered, this, &FrmWidget::on_singleLoopBtn);
    QPoint tempPos = QPoint(QCursor::pos().x(), QCursor::pos().y()-160);
    popMenu.exec(tempPos);//弹出右键菜单，菜单位置为光标位置
}

void FrmWidget::on_singleBtn()
{
    IconStyle::Instance()->setIcon(ui->playOrderBtn, QChar(0xf178), 17);
    single->setChecked(true);
}

void FrmWidget::on_sequenceBtn()
{
    IconStyle::Instance()->setIcon(ui->playOrderBtn, QChar(0xf0cb), 17);
    sequence->setChecked(true);
}

void FrmWidget::on_listLoopBtn()
{
    IconStyle::Instance()->setIcon(ui->playOrderBtn, QChar(0xf079), 17);
    listLoop->setChecked(true);
}

void FrmWidget::on_randomBtn()
{
    IconStyle::Instance()->setIcon(ui->playOrderBtn, QChar(0xf074), 17);
    random->setChecked(true);
}

void FrmWidget::on_singleLoopBtn()
{
    IconStyle::Instance()->setIcon(ui->playOrderBtn, QChar(0xf01e), 17);
    singleLoop->setChecked(true);
}

void FrmWidget::on_playList_doubleClicked(const QModelIndex &index)
{
    QString fileName = index.data().toString();
    ui->label_title->setText(fileName);
    on_play();
}

void FrmWidget::on_listHideBtn_clicked()
{

}
