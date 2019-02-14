#include "qsingleapplication.h"
#include <QFileInfo>
#include<QLocalSocket>
#define TIME_OUT                (500)

QSingleApplication::QSingleApplication(int &argc, char **argv):QApplication(argc, argv)
  , w(nullptr)
  , is_Running(false)
  , localServer(nullptr)
{
    serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    initLocalConnection();
}

bool QSingleApplication::isRunning() {
    return is_Running;
}

// 通过socket通讯实现程序单实例运行，监听到新的连接时触发该函数
void QSingleApplication::newLocalConnection() {
    QLocalSocket *socket = localServer->nextPendingConnection();
    if(socket) {
        socket->waitForReadyRead(2*TIME_OUT);
        delete socket;
        // 其他处理，如：读取启动参数
        activateWindow();
    }
}

// 通过socket通讯实现程序单实例运行，初始化本地连接，如果连接不上server，则创建，否则退出
void QSingleApplication::initLocalConnection() {
    is_Running = false;

    QLocalSocket socket;
    socket.connectToServer(serverName);
    if(socket.waitForConnected(TIME_OUT)) {//如果连接的话
        fprintf(stderr, "%s already running.\n",
                serverName.toLocal8Bit().constData());
        is_Running = true;
        // 其他处理，如：将启动参数发送到服务端
        return;
    }

    //连接不上服务器，就创建一个
    newLocalServer();
}

// 创建LocalServer
void QSingleApplication::newLocalServer() {
    localServer = new QLocalServer(this);
    connect(localServer, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
    if(!localServer->listen(serverName)) {
        // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
        if(localServer->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(serverName); // <-- 重点
            localServer->listen(serverName); // 再次监听
        }
    }
}

// 激活主窗口
void QSingleApplication::activateWindow() {
    if(w) {
        w->showNormal();
        w->activateWindow(); // 激活窗口
        w->raise();
    }
}
