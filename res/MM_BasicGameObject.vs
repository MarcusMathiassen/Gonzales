#version 410

in vec3 position;
in vec2 textCoord;
in vec3 normal;

uniform mat4 transform;

out Vertex
{
  vec2 textCoord;
  vec3 normal;
} vertex;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    vertex.textCoord = textCoord;
    vertex.normal = (transform * vec4(normal, 0.0)).xyz;
}
