#include "MeshShader.h"
#include "App/AppSettings.h"

MeshShader::MeshShader()
{
	if (!loadShaders("Resources/Shaders/BasicMesh.vert", "Resources/Shaders/BasicMesh.frag", "Resources/Shaders/basicBackCull.geom"))
	{
		system("pause");
	}
	use();
	m_modelMatrixLocation = glGetUniformLocation(program(), "modelMatrix");
	m_viewMatrixLocation = glGetUniformLocation(program(), "viewMatrix");
	m_projectionMatrixLocation = glGetUniformLocation(program(), "projectionMatrix");
	m_cameraPositionLocation = glGetUniformLocation(program(), "cameraPos");
	unuse();
}

MeshShader::~MeshShader()
{
}

void MeshShader::setModelMatrix(const glm::mat4 & model)
{
	glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
}

void MeshShader::setViewMatrix(const glm::mat4 & view)
{
	glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &view[0][0]);
}

void MeshShader::setCameraPosition(const glm::vec3 & position)
{
	glUniform3f(m_cameraPositionLocation, position.x, position.y, position.z);
}

void MeshShader::setProjectionMatrix(const glm::mat4 & proj)
{
	glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &proj[0][0]);
}
