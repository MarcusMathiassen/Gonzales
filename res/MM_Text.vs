#version 410

in vec2 position;
in vec2 textCoord;

uniform mat4 model;

out Vertex
{
  vec4 color;
  vec2 textCoord;
} vertex;

void main()
{
  gl_Position =  model * vec4(position, 0, 1.0);
  vertex.textCoord = textCoord;
}
