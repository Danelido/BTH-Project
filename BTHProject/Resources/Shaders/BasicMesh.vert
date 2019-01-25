#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 cameraPos;

out vec3 frag_normal;
out vec2 frag_uv;
out vec3 frag_objectPos;
out vec3 frag_cameraPos;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    frag_normal = normal;
    frag_uv = uv;
	frag_cameraPos = cameraPos;
	frag_objectPos = vec3(modelMatrix * vec4(position, 1.0f));
	
}