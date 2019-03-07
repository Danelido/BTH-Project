#version 430

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 frag_normal;
in vec2 frag_uv;
in vec3 frag_objectPos;

uniform float shininess;
uniform int selected;

uniform sampler2D textureSampler;

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);

	if(selected == 1)
		textureColor = textureColor * vec4(0.8f, 0.8f, 0.8f, 1.0f) + vec4(0.f, 1.f - textureColor.g , 0.f, 0.f);

	gPosition = frag_objectPos;	
	gNormal = normalize(frag_normal);
	gAlbedoSpec.rgb = textureColor.rgb;
	gAlbedoSpec.a = shininess;
}