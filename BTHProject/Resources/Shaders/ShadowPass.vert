#version 430
layout (location = 0) in vec3 position;

uniform mat4 modelMatrix = mat4(1.f);
uniform mat4 lightTransform;

void main()
{
    gl_Position = lightTransform * modelMatrix *vec4(position, 1.0f);
}
