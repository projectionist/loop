extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
}

#include <loop/dbg.h>
#include <loop/video.hpp>

namespace loop {
  int video_open_file(const shared_ptr<loop::playback> pb) {

    const char *filename = pb->filename.c_str();

    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *pFrame = NULL;
    AVPacket packet;
    int frameFinished;
    int videoStream;
    int rc;
    unsigned int i;

    rc = avformat_open_input(&pFormatCtx, filename, 0, NULL);
    if(rc != 0) {
      log_err("Can't open %s", filename);
      return rc;
    }

    rc = avformat_find_stream_info(pFormatCtx, NULL);
    if(rc != 0) {
      log_err("Can't get stream info. (%s)", filename);
      return rc;
    }

    av_dump_format(pFormatCtx, 0, filename, 0);

    videoStream = -1;
    for(i = 0; i < pFormatCtx->nb_streams; i++) {
      if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
        videoStream = i;
        break;
      }
    }
    if(videoStream == -1) {
      log_err("No video stream. (%s)", filename);
      return -1;
    }

    // get the codec context for the video stream
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    // set the time_base on the shared playback context
    pb->time_base = av_q2d(pCodecCtx->time_base);

    // find the decoder for the video stream
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL) {
      log_err("unsupported codec. (%s)", filename);
      return -1;
    }

    // open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
      log_err("can't open codec. (%s)", filename);
      return -1; // Could not open codec
    }

    // Allocate video frame
    pFrame = av_frame_alloc();

    while(av_read_frame(pFormatCtx, &packet) >= 0) {
      if(packet.stream_index == videoStream) {
        // Decode video frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        if(frameFinished) {
          AVFrame *pFrame_copy = av_frame_clone(pFrame);
          if(pFrame_copy == NULL) {
            log_err("frame clone failed. (%s)", filename);
            continue;
          }

          pb->buffer_ptr->enqueue(pFrame_copy);
        }
      }

      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);
    }

    // Free the YUV frame
    av_frame_free(&pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 0;
  }
}
