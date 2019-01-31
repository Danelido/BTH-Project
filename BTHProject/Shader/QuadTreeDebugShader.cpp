#include "QuadTreeDebugShader.h"

QuadTreeDebugShader::QuadTreeDebugShader()
{
	if (!loadShaders("Resources/Shaders/QuadTreeDebug.vert", "Resources/Shaders/QuadTreeDebug.frag", nullptr))
	{
		system("pause");
	}
	use();
	m_modelMatrixLocation = glGetUniformLocation(program(), "modelMatrix");
	m_viewMatrixLocation = glGetUniformLocation(program(), "viewMatrix");
	m_projectionMatrixLocation = glGetUniformLocation(program(), "projectionMatrix");
	m_colorLocation = glGetUniformLocation(program(), "color");
	unuse();
}

QuadTreeDebugShader::~QuadTreeDebugShader()
{
	
}

void QuadTreeDebugShader::setModelMatrix(const glm::mat4 & model)
{
	glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
}

void QuadTreeDebugShader::setViewMatrix(const glm::mat4 & view)
{
	glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &view[0][0]);
}

void QuadTreeDebugShader::setProjectionMatrix(const glm::mat4 & proj)
{
	glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &proj[0][0]);
}

void QuadTreeDebugShader::setColor(const glm::vec3 & color)
{
	glUniform3f(m_colorLocation, color.r, color.g, color.b);
}
