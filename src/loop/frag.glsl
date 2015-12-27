#version 100
precision mediump float;

uniform sampler2D frame_texture;

varying vec2 v_texCoord;

void main() {
  gl_FragColor = texture2D(frame_texture, vec2(v_texCoord.x, v_texCoord.y));
}
