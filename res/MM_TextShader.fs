#version 410

out vec4 fragColor;

uniform sampler2D tex;

in Vertex
{
  vec2 textCoord;
  vec4 color;
} frag;

void main()
{
  fragColor = texture(tex, frag.textCoord) * frag.color;
}
