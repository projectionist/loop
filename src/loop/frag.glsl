#version 100
precision mediump float;

uniform sampler2D frame_texture;

varying vec2 v_texCoord;

void main() {
  vec4 rgba = texture2D(frame_texture, vec2(v_texCoord.x, v_texCoord.y));

  float d = 1.0 - rgba.g;
  d = pow(d, 5.0);

  rgba.g += d;

  if (rgba.r > rgba.g || rgba.b > rgba.g) {
    rgba.g = 0.0;
  }
  gl_FragColor = rgba;
}
