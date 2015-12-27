#pragma once

#include <memory>
#include <vector>

#include <GLES2/gl2.h>

#include <projection/drawing.hpp>
#include <projection/program_helper.hpp>
#include <loop/playback.hpp>

namespace scratch {
  class scratch : public projection::drawing {
    GLuint program;
    GLuint vbo;
    std::unique_ptr<projection::program_helper> program_helper;
    std::shared_ptr<loop::playback> video_playback;
    GLuint texture_ids[3];
  public:
    void setup();
    void draw();
    void prepare_textures();
    explicit scratch(std::shared_ptr<loop::playback> _video_playback)
      :video_playback{_video_playback}
    {
    }
  };
}
