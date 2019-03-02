#include "ParticleShader.h"

ParticleShader::ParticleShader()
{
	if (!loadShaders("Resources/Shaders/Particles.vert", "Resources/Shaders/Particles.frag", nullptr))
	{
		system("pause");
	}
	use();
	m_viewMatrixLocation = glGetUniformLocation(program(), "viewMatrix");
	m_projectionMatrixLocation = glGetUniformLocation(program(), "projectionMatrix");
	unuse();
}

ParticleShader::~ParticleShader()
{
}

void ParticleShader::setViewMatrix(const glm::mat4 & view)
{
	glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &view[0][0]);
}

void ParticleShader::setProjectionMatrix(const glm::mat4 & proj)
{
	glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &proj[0][0]);
}
