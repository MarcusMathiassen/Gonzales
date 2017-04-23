#version 410

out vec4 frag_color;

in vec2 textCoord0;
flat in vec3 normal0;

uniform sampler2D diffuse;

uniform int wireframe;
uniform vec4 color;

void main()
{
  // Diffuse texture
  //vec4 diff = texture(diffuse, textCoord0) * max(0.0, dot(-vec3(0,0,1), normal0));

  // diffuse color
  vec4 diff = vec4(1,1,1,1) * max(0.0, dot(-vec3(0,0,1), normal0));
  if (wireframe == 1) frag_color = vec4(1,1,1,1);
  else frag_color = diff;
}
