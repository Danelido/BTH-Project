#version 430

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 frag_normal;
in vec2 frag_uv;
in vec3 frag_objectPos;

uniform sampler2D textureSampler;

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);
	gPosition = frag_objectPos;	
	gNormal = normalize(frag_normal);
	gAlbedoSpec.rgb = textureColor.rgb;
	gAlbedoSpec.a = 0.0f;  // Read from specular texture here when you have one ( The r value )
}