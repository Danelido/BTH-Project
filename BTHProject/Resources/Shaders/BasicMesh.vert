#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out VS_DATA
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_data;


void main()
{
    vs_data.position = position;
    vs_data.normal = normal;
	vs_data.uv = uv;
}