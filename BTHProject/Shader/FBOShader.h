#ifndef _FRAMEBUFFER_OBJECT_SHADER_H
#define _FRAMEBUFFER_OBJECT_SHADER_H
#include "Shader.h"
#include <GLM/glm.hpp>
#include "Light/Light.h"


class FBOShader : public Shader
{
public:
	FBOShader();
	~FBOShader();

	void setCameraPosition(const glm::vec3& position);
	void setSunPosition(const glm::vec3& position);
	void registerLights(const std::vector<Light*>* lights);
	void setLightTransform(const glm::mat4& lightTransform);
	void setShadowBiaz(const float& biaz);
private:
	GLint m_cameraPositionLocation;
	GLint m_numberOfLightsLocation;
	GLint m_sunPositionLocation;
	GLint m_lightTransformLocation;
	GLint m_shadowBiazLocation;

	struct lightShaderProperties
	{
		GLint positionLocation;
		GLint colorLocation;
		GLint radiusLocation;
	};

	std::vector<lightShaderProperties> m_lightShaderProperties;
};

#endif