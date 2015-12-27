#version 100
precision mediump float;

uniform sampler2D frame_texture;
uniform float snap_t;

varying vec2 v_texCoord;

void main() {
  vec4 rgba = texture2D(frame_texture, vec2(v_texCoord.x, v_texCoord.y));

  float lum = (2.0 * 0.2989 * rgba.r) + (0.5870 * rgba.g) + (0.5 * 0.1140 * rgba.b);
  lum = 1.0 - lum;

  if (lum > 0.6) {
    lum = 1.0;
  } else {
    lum = 0.0;
  }

  gl_FragColor = vec4(lum, lum, lum , 1.0);
}
