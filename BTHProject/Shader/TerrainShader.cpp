#include "TerrainShader.h"

TerrainShader::TerrainShader()
{
	if (!loadShaders("Resources/Shaders/TerrainShader.vert", "Resources/Shaders/TerrainShader.frag", "Resources/Shaders/TerrainBackCull.geom"))
	{
		system("pause");
	}
	use();
	m_modelMatrixLocation = glGetUniformLocation(program(), "modelMatrix");
	m_viewMatrixLocation = glGetUniformLocation(program(), "viewMatrix");
	m_projectionMatrixLocation = glGetUniformLocation(program(), "projectionMatrix");
	m_cameraPositionLocation = glGetUniformLocation(program(), "cameraPos");

	glUniform1i(glGetUniformLocation(program(), "blendMap"), 3);
	glUniform1i(glGetUniformLocation(program(), "backgroundTexture"), 4);
	glUniform1i(glGetUniformLocation(program(), "rTexture"), 5);
	glUniform1i(glGetUniformLocation(program(), "gTexture"), 6);
	glUniform1i(glGetUniformLocation(program(), "bTexture"), 7);
	unuse();
}

TerrainShader::~TerrainShader()
{
}

void TerrainShader::setModelMatrix(const glm::mat4 & model)
{
	glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
}

void TerrainShader::setViewMatrix(const glm::mat4 & view)
{
	glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &view[0][0]);
}

void TerrainShader::setProjectionMatrix(const glm::mat4 & proj)
{
	glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &proj[0][0]);
}

void TerrainShader::setCameraPosition(const glm::vec3 & position)
{
	glUniform3f(m_cameraPositionLocation, position.x, position.y, position.z);
}
