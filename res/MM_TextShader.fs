#version 410

out vec4 fragColor;

uniform sampler2D tex;

in Vertex
{
  vec2 textCoord;
} frag;

void main()
{
  fragColor = texture(tex, frag.textCoord);
  fragColor = vec4(1,1,1,1);
}
