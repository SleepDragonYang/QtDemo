#ifndef QSINGLEAPPLICATION_H
#define QSINGLEAPPLICATION_H
#include <QApplication>
#include <QObject>
#include <QLocalServer>
#include <QWidget>


class QSingleApplication : public QApplication
{
    Q_OBJECT
public:
    QSingleApplication(int &argc, char **argv);

    bool isRunning();
    QWidget *w;
private slots:
    // 有新连接时触发
    void newLocalConnection();

private:
    // 初始化本地连接
    void initLocalConnection();
    // 创建服务端
    void newLocalServer();
    // 激活窗口
    void activateWindow();

    bool is_Running;                // 是否已经有实例在运行
    QLocalServer *localServer;     // 本地socket Server
    QString serverName;            // 服务名称

};

#endif // QSINGLEAPPLICATION_H
