#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QObject>
#include <QImage>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avutil.h"
#include "libpostproc/postprocess.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
}
class VideoThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoThread(QObject *parent = nullptr);
    void setFilePath(const QString &fileName);
signals:
    void sig_GetOneFrame(QImage);
    void sig_Error(QString);
public slots:
protected:
    void run();
private:
    QString m_fileName;
};

#endif // VIDEOTHREAD_H
