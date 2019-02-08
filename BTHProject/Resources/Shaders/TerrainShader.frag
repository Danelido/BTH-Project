#version 430

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 frag_normal;
in vec2 frag_uv;
in vec3 frag_objectPos;

layout (location = 4) uniform sampler2D blendMap;
layout (location = 3) uniform sampler2D backgroundTexture;
layout (location = 5) uniform sampler2D rTexture;
layout (location = 6) uniform sampler2D gTexture;
layout (location = 7) uniform sampler2D bTexture;


void main()
{
	vec4 blendMapColor = texture(blendMap, frag_uv);
	float backgroundTextureFactor = 1.f - (blendMapColor.r + blendMapColor.g + blendMapColor.b);

	vec2 newUV = frag_uv * 40;
	
	vec4 backgroundColor = texture(backgroundTexture, newUV) * backgroundTextureFactor;
	vec4 rTextureColor = texture(rTexture, newUV) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, newUV) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, newUV) * blendMapColor.b;

	vec4 color = backgroundColor + rTextureColor + gTextureColor + bTextureColor;

	gPosition = frag_objectPos;	
	gNormal = normalize(frag_normal);
	gAlbedoSpec.rgb = color.rgb;
	gAlbedoSpec.a = 0.0f;  // Read from specular texture here when you have one ( The r value )
}