#version 430

in vec2 frag_uv;
out vec4 finalColor;

layout (location = 0) uniform sampler2D gPosition;
layout (location = 1) uniform sampler2D gNormal;
layout (location = 2) uniform sampler2D gAlbedoSpec;

struct Light
{
	vec3 position;
	vec3 color;
	float radius;
};

const int MAX_LIGHTS = 330;
uniform int nrOfLights;
uniform Light lights[MAX_LIGHTS];
uniform vec3 cameraPos;

uniform vec3 sunPosition;
vec3 sunColor = vec3(0.9f,0.9f,0.9f);

void main()
{
	// Get all the values
	vec3 position = texture(gPosition,frag_uv).rgb;
	vec3 normal = texture(gNormal,frag_uv).rgb;
	vec3 albedo = texture(gAlbedoSpec,frag_uv).rgb;
	float specular = texture(gAlbedoSpec,frag_uv).a; 

	// Ambient color
	float ambientFactor = 0.2f;
	vec3 ambient = vec3(1.f) * ambientFactor;
	vec4 currentColor = vec4(ambient,1.0f);


	// Sun
		/* DIFFUSE */
		vec3 toLight = sunPosition - position;
		float factor = dot(normalize(toLight), normalize(normal));
		factor = max(factor, 0.0f);
		
		vec3 diffuse = sunColor * factor;

		/* FINAL COLOR */
		currentColor += vec4((diffuse), 0.f);
		

	// Every other light
	for(int i = 0; i < nrOfLights; i++)
	{
		float dist = length((lights[i].position - position));
		float strengthFactor = clamp((lights[i].radius - dist) / (lights[i].radius), 0.f, 1.f);
	
		if(strengthFactor <= 0.04f)
		{
			continue;
		}

		/* DIFFUSE */
		vec3 toLight = lights[i].position - position;
		float factor = dot(normalize(toLight), normalize(normal));
		factor = max(factor, 0.0f);
		vec3 diffuse = lights[i].color * factor * strengthFactor;

		// TODO: Phong


		/* FINAL COLOR */
		currentColor += vec4((diffuse), 0.f);
	}

	// Clamp it
	finalColor = min(currentColor, vec4(1.f, 1.f, 1.f,1.f)) * vec4(albedo, 1.0f);
	
}

