#version 410

in vec2 position;
in vec2 textCoord;

uniform vec2 pos;
uniform vec2 viewport;

out Vertex
{
  vec4 color;
  vec2 textCoord;
} vertex;

void main()
{
  float inverseAspectRatio = 1.0 / (viewport.x / viewport.y);
  vec2 fPos = vec2(position.x * inverseAspectRatio, position.y);
  gl_Position =  vec4(fPos+pos, -1.0, 1.0);
  vertex.textCoord = textCoord;
}
