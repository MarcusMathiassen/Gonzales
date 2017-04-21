#version 410

out vec4 fragColor;

uniform sampler2D diffuse;

in Vertex
{
  vec4 color;
  vec2 textCoord;
} frag;

void main()
{
  vec4 character = texture(diffuse, frag.textCoord, 0);
  fragColor = character * vec4(1,1,1,1);
}
