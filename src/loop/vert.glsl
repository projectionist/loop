#version 100
attribute vec2 a_position;
varying vec2 v_texCoord;

void main()
{
  v_texCoord = vec2((a_position.x / 2.0) + 0.5, (-a_position.y / 2.0) + 0.5);
  gl_Position = vec4(a_position, 0.0, 1.0);
}
