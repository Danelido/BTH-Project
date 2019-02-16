#ifndef _SHADOW_MAP_SHADER_H
#define _SHADOW_MAP_SHADER_H
#include "Shader.h"
#include <GLM/glm.hpp>

class ShadowMapShader : public Shader {

public:

	ShadowMapShader();
	virtual ~ShadowMapShader();

	void setModelMatrix(const glm::mat4& model);
	void setLightTransform(const glm::mat4& lightTransform);

private:
	GLuint m_lightTransformLocation;
	GLuint m_modelMatrixLocation;


};


#endif
