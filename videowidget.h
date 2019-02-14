#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QPainter>
#include <QImage>
class VideoWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
    void setImage(const QImage &image);
signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
private:
    QImage m_image;
};

#endif // VIDEOWIDGET_H
