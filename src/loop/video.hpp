#pragma once

#include <queue>
#include <thread>
#include <condition_variable>

#include <loop/playback.hpp>

namespace loop {
  int video_open_file(const shared_ptr<playback>);
}
