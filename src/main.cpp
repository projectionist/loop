extern "C" {
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
  #include <libavutil/avutil.h>
}

#include <csignal>
#include <memory>
#include <iostream>
#include <thread>

#include <projection/play.hpp>
#include <loop/scratch.hpp>
#include <loop/video.hpp>

using namespace std;

void video_loop(shared_ptr<loop::playback> pb_ptr) {
  while (true) {
    auto rc = loop::video_open_file(pb_ptr);
    if (rc != 0) {
      cerr << "Playback failed" << endl;
      terminate();
    }
  }
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    cout << "Video path argument required" << endl;
    cout << "EG: bin/main <video-path>" << endl;
    return EXIT_FAILURE;
  }

  av_register_all();
  auto pb_ptr = make_shared<loop::playback>(argv[1]);

  std::thread t_decode{video_loop, pb_ptr};

  t_decode.detach();

  scratch::scratch *s = new scratch::scratch(pb_ptr);
  shared_ptr<projection::drawing> drawing = shared_ptr<projection::drawing>(s);
  projection::play(drawing);

  pb_ptr->buffer_ptr->close();

  cout << "frame rate: " << drawing->frame_rate() << "f/s" << endl;

  return EXIT_SUCCESS;
}
