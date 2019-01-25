#ifndef _FRAMEBUFFER_OBJECT_SHADER_H
#define _FRAMEBUFFER_OBJECT_SHADER_H
#include "Shader.h"
#include <GLM/glm.hpp>
#include "Entity/Light.h"


class FBOShader : public Shader
{
public:
	FBOShader();
	~FBOShader();

	void setCameraPosition(const glm::vec3& position);
	void setSunPosition(const glm::vec3& position);
	void registerLights(const std::vector<Light*>* lights);

private:
	GLint m_cameraPositionLocation;
	GLint m_numberOfLightsLocation;
	GLint m_sunPositionLocation;

	struct lightShaderProperties
	{
		GLint positionLocation;
		GLint colorLocation;
	};

	std::vector<lightShaderProperties> m_lightShaderProperties;
};

#endif