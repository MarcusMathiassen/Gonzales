#version 410

out vec4 fragColor;

uniform sampler2D diffuse;

//uniform int wireframe;
//uniform vec4 color;

in Vertex
{
  vec2 textCoord;
  vec3 normal;
} frag;

void main()
{
  // Diffuse texture
  vec4 diff = texture(diffuse, frag.textCoord) * max(0.0, dot(-vec3(0,0,1), frag.normal));

  // diffuse color
  //vec4 diff = vec4(1,1,1,1) * max(0.0, dot(-vec3(0,0,1), frag.normal));
  //if (wireframe == 1) fragColor = vec4(1,1,1,1);
  fragColor = diff;
}

