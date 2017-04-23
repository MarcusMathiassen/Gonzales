#version 410

out vec4 fragColor;

uniform sampler2D diffuse;

in Vertex
{
  vec2 textCoord;
  vec3 normal;
} frag;

void main()
{
  vec4 tex = texture(diffuse, frag.textCoord);
  fragColor = tex * max(0.0, dot(-vec3(0,0,1), frag.normal));
}

