#version 410

in vec2 position;
in vec2 textCoord;

uniform vec2 pos;

out Vertex
{
  vec4 color;
  vec2 textCoord;
} vertex;

void main()
{
  gl_Position =  vec4(position+pos, 0, 1.0);
  vertex.textCoord = textCoord;
}
