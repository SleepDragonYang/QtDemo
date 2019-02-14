#include "videothread.h"
#include <QDebug>
VideoThread::VideoThread(QObject *parent) : QThread(parent)
{

}

void VideoThread::setFilePath(const QString &fileName)
{
    if(!fileName.isEmpty() | !fileName.isNull())
        m_fileName = fileName;
}

void VideoThread::run()
{
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext *pCodecCtx = nullptr;
    AVCodecParameters *pCodecpar = nullptr;
    AVCodec *pCodec = nullptr;
    AVFrame *pFrameIn = nullptr;
    AVFrame *pFrameOut = nullptr;
    AVPacket *packet = nullptr;
    uint8_t *frame_buffer_out = nullptr;
    uint8_t *frame_buffer_in = nullptr;

    static struct SwsContext *img_convert_ctx = nullptr;

    int videoStream = 0;

    av_register_all(); //初始化FFMPEG  调用了这个才能正常适用编码器和解码器

    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, m_fileName.toStdString().data(), nullptr, nullptr) != 0) {
        printf("can't open the file. ");
        emit sig_Error("Can't open the file.");
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        printf("Could't find stream infomation.");
        emit sig_Error("Could't find stream infomation.");
        return;
    }


    videoStream = -1;

    ///循环查找视频中包含的流信息，直到找到视频类型的流
    ///便将其记录下来 保存到videoStream变量中
    ///这里我们现在只处理视频流  音频流先不管他
    int nb_streams = static_cast<int>(pFormatCtx->nb_streams);
    for (int i = 0; i < nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }
    qDebug()<<pFormatCtx->bit_rate;
    ///如果videoStream为-1 说明没有找到视频流
    if (videoStream == -1) {
        printf("Could't find a video stream.");
        emit sig_Error("Could't find a video stream.");
        return;
    }

    ///查找解码器
    if (videoStream == -1)
        return ; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecpar = pFormatCtx->streams[videoStream]->codecpar;

    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id);
    if (pCodec == nullptr) {
        fprintf(stderr, "Unsupported codec!\n");
        return ; // Codec not found
    }


    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id);

    if (pCodec == nullptr) {
        printf("Codec not found.");
        emit sig_Error("Codec not found.");
        return;
    }
    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->time_base.num = 1;
    pCodecCtx->frame_number = 1; //每包一个视频帧
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->bit_rate = 0;
    pCodecCtx->time_base.den = 30;//帧率
    pCodecCtx->width = 1280;//视频宽
    pCodecCtx->height = 720;//视频高
    pCodecCtx->pix_fmt = AV_PIX_FMT_RGB32;
    if (avcodec_parameters_to_context(pCodecCtx, pCodecpar) != 0) {
        fprintf(stderr, "Couldn't copy codec context");
        return; // Error copying codec context
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
        return; // Could not open codec

    pFrameOut = av_frame_alloc();
    pFrameIn= av_frame_alloc();

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, nullptr, nullptr);

    unsigned int numBytes = static_cast<unsigned int>(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));

    frame_buffer_out = reinterpret_cast<uint8_t *>(av_malloc(numBytes * sizeof(uint8_t)));

    av_image_fill_arrays(pFrameOut->data, pFrameOut->linesize, frame_buffer_out, AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

    int y_size = pCodecCtx->width * pCodecCtx->height;
    packet = reinterpret_cast<AVPacket *>(malloc(sizeof(AVPacket))); //分配一个packet

    av_new_packet(packet, y_size); //分配packet的数据
    av_dump_format(pFormatCtx, 0, m_fileName.toStdString().data(), 0); //输出视频信息
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
         // Is this a packet from the video stream?
         if (packet->stream_index == videoStream)
         {
             // Decode video frame
             avcodec_send_packet(pCodecCtx, packet);
             if (avcodec_receive_frame(pCodecCtx, pFrameIn) != 0)
                 continue;

             sws_scale(img_convert_ctx,
                       reinterpret_cast<uint8_t const * const *>(pFrameIn->data),
                       pFrameIn->linesize, 0, pCodecCtx->height, pFrameOut->data,
                       pFrameOut->linesize);

             //把这个RGB数据 用QImage加载
             QImage tmpImg(reinterpret_cast<uchar*>(frame_buffer_out), pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32);
             QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
             emit sig_GetOneFrame(image);  //发送信号
             msleep(25);
         }
        av_packet_unref(packet);
    }


    av_free(frame_buffer_in);
    av_free(frame_buffer_out);
    av_free(pFrameIn);
    av_free(pFrameOut);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
