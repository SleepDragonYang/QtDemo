#include "frmwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>
#include "qsingleapplication.h"
int main(int argc, char *argv[])
{
    QSingleApplication app(argc, argv);

    if(!app.isRunning())
    {
        //加载样式表
        QFile file(":/qss/skin.css");
        if (file.open(QFile::ReadOnly)) {
            QString qss = QLatin1String(file.readAll());
            QString paletteColor = qss.mid(20, 7);
            qApp->setPalette(QPalette(QColor(paletteColor)));
            qApp->setStyleSheet(qss);
            file.close();
        }
        app.setFont(QFont("Microsoft Yahei", 9));
        // 添加字体文件

        FrmWidget w;
        w.resize(1100, 660);
        QDesktopWidget* desktop = QApplication::desktop(); //居中显示
        w.move((desktop->width() - w.width())/2, (desktop->height() - w.height())/2);
        w.show();
        return app.exec();
    }
    return app.exec();

}
