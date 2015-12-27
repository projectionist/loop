#pragma once

extern "C" {
  #include <libavutil/frame.h>
}

#include <memory>
#include <atomic>
#include <string>

#include <loop/buffer.hpp>

namespace loop {
  class playback {
  public:
    const std::string filename;
    std::atomic<double> time_base;
    std::shared_ptr<loop::buffer<AVFrame*>> buffer_ptr;
    explicit playback(std::string filename);
  };
}
