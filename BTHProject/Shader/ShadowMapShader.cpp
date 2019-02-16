#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader()
{
	if (!loadShaders("Resources/Shaders/ShadowPass.vert", "Resources/Shaders/ShadowPass.frag", nullptr))
	{
		system("pause");
	}
	use();
	m_modelMatrixLocation = glGetUniformLocation(program(), "modelMatrix");
	m_lightTransformLocation = glGetUniformLocation(program(), "lightTransform");
	unuse();
}

ShadowMapShader::~ShadowMapShader()
{

}

void ShadowMapShader::setModelMatrix(const glm::mat4 & model)
{
	glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
}


void ShadowMapShader::setLightTransform(const glm::mat4 & lightTransform)
{
	glUniformMatrix4fv(m_lightTransformLocation, 1, GL_FALSE, &lightTransform[0][0]);
}

