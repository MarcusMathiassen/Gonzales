#version 410

in vec3 position;
in vec2 textCoord;
in vec3 normal;

out vec2 textCoord0;
flat out vec3 normal0;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    textCoord0 = textCoord;
    normal0 = (transform * vec4(normal, 0.0)).xyz;
}
