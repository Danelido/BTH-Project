#include "InstancedMeshShader.h"

InstancedMeshShader::InstancedMeshShader()
{
	if (!loadShaders("Resources/Shaders/InstancedMesh.vert", "Resources/Shaders/InstancedMesh.frag", "Resources/Shaders/instancedBackCull.geom"))
	{
		system("pause");
	}
	use();
	m_viewMatrixLocation = glGetUniformLocation(program(), "viewMatrix");
	m_projectionMatrixLocation = glGetUniformLocation(program(), "projectionMatrix");
	m_cameraPositionLocation = glGetUniformLocation(program(), "cameraPos");
	
	unuse();
}


InstancedMeshShader::~InstancedMeshShader()
{
}

void InstancedMeshShader::setCameraPosition(const glm::vec3& position)
{
	glUniform3f(m_cameraPositionLocation, position.x, position.y, position.z);
}

void InstancedMeshShader::setViewMatrix(const glm::mat4 & view)
{
	glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &view[0][0]);
}

void InstancedMeshShader::setProjectionMatrix(const glm::mat4 & proj)
{
	glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &proj[0][0]);
}
