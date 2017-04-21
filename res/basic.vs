#version 410

in vec2 position;
uniform vec2 pos;

void main()
{
  gl_Position = vec4(position.xy+pos, 0.0, 1.0);
}
