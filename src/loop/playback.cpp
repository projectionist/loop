extern "C" {
  #include <libavutil/frame.h>
}

#include <memory>
#include <string>

#include <loop/playback.hpp>

namespace loop {
  playback::playback(std::string filename):
    filename{filename},
    buffer_ptr{new buffer<AVFrame*>(10)}
  {}
}
