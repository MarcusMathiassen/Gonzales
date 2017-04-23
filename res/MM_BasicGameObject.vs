#version 410

in vec3 position;
in vec2 textCoord;
in vec3 normal;

uniform mat4 MVP;

out Vertex
{
  vec2 textCoord;
  vec3 normal;
} vertex;

void main()
{
  gl_Position = MVP * vec4(position, 1.0);

  vertex.textCoord = textCoord;
  vertex.normal = (MVP * vec4(normal, 0.0)).xyz;
}
