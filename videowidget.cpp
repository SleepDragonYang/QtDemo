#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}
void VideoWidget::setImage(const QImage &image)
{
    m_image = image;
    update();
}
void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    if (m_image.size().width() <= 0) return;

    ///将图像按比例缩放成和窗口一样大小
    QImage img = m_image.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawImage(this->geometry(), img); //画出图像

}
