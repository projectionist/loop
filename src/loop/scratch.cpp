extern "C" {
  #include <libavutil/frame.h>
  #include <libswscale/swscale.h>
}

#include <cassert>
#include <ctime>
#include <cstdio>
#include <unistd.h>
#include <cmath>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <loop/scratch.hpp>

using namespace std;

namespace scratch {

  void scratch::setup()
  {
    program_helper = make_unique<projection::program_helper>("./src/loop/vert.glsl", "./src/loop/frag.glsl");
    program = program_helper->program();

    /*
      if you bind attribute names to locations (glBindAttribLocation), do it before linking
    */
    program_helper->link();
    /*
      if you get attribute/uniform locations (glGetAttribLocation), do it after linking
    */

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program);

    glGenBuffers(1, &vbo);
    glGenTextures(1, &texture_id);

    threshold = 1.0;
    threshold_target = 1.0;
    update();
  }

  void scratch::prepare_textures()
  {
    AVFrame * frame = NULL;

    try {
      frame = video_playback->buffer_ptr->dequeue();
    } catch (loop::buffer_closed) {
      return;
    }

    auto frameWidth = frame->width;
    auto frameHeight = frame->height;

    SwsContext * ctx = sws_getContext(frameWidth, frameHeight,
                                      AV_PIX_FMT_YUV420P,
                                      frameWidth, frameHeight,
                                      AV_PIX_FMT_RGB24,
                                      0, 0, 0, 0);

    uint8_t * rgb888 = new uint8_t[3 * frameWidth * frameHeight];
    uint8_t * out_data[1] = { rgb888 }; // RGB24 have one plane
    int out_linesize[1] = { 3 * (frameWidth) }; // RGB stride
    sws_scale(ctx, frame->data, frame->linesize, 0, frameHeight, out_data, out_linesize);

    av_frame_free(&frame);
    sws_freeContext(ctx);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb888);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glUniform1i(glGetUniformLocation(program, "frame_texture"), 0);

    delete rgb888;

    usleep(double(video_playback->time_base) * 1024 * 1024);
  }

  void scratch::update()
  {
    double time = elapsed();

    if (threshold == threshold_target) {
      threshold_target = 0.5 + (0.35 * ((float) rand() / (float) RAND_MAX));
      new_target_time = elapsed();
    }

    auto diff = abs(threshold - threshold_target);
    auto time_diff = pow(min(0.2 * (time - new_target_time), 1.0), 2.0);
    auto adjust = time_diff * diff;

    if (threshold_target < threshold) {
      adjust *= -1.0;
    }

    threshold += adjust;
  }

  void scratch::draw()
  {
    glViewport(0, 0, width(), height());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    update();
    prepare_textures();

    GLfloat verticies[] = {
       -1.0f, 1.0f,
       -1.0f, -1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    auto a_pos_location = glGetAttribLocation(program, "a_position");

    glUniform1f(glGetUniformLocation(program, "snap_t"), threshold);

    // Load the vertex data
    glVertexAttribPointer(a_pos_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(a_pos_location);
    assert(glGetError() == GL_NO_ERROR);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}
