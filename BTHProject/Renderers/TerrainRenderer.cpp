#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(const glm::mat4 & projectionMatrix)
{
	m_terrainShader = new TerrainShader();
	m_terrainShader->use();
	m_terrainShader->setProjectionMatrix(projectionMatrix);
	m_terrainShader->unuse();

	m_terrains.reserve(100);
}

TerrainRenderer::~TerrainRenderer()
{
	delete m_terrainShader;
}

void TerrainRenderer::submit(Terrain * terrain)
{
	m_terrains.emplace_back(terrain);
}

void TerrainRenderer::render(const FPSCamera * camera)
{
	m_terrainShader->use();

	for (size_t i = 0; i < m_terrains.size(); i++)
	{
		bindMesh(m_terrains[i], camera);
		glDrawElements(GL_TRIANGLES, m_terrains[i]->getTerrainMesh()->getIndicesSize(), GL_UNSIGNED_INT, NULL);
		unbindMesh();
	}

	m_terrainShader->unuse();
}

void TerrainRenderer::bindMesh(Terrain * terrain, const FPSCamera * camera)
{
	glBindVertexArray(terrain->getTerrainMesh()->getVao());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	m_terrainShader->setCameraPosition(camera->getPosition());
	m_terrainShader->setModelMatrix(terrain->getModelMatrix());
	m_terrainShader->setViewMatrix(camera->getViewMatrix());

	
	for (int i = 0; i < terrain->getTerrainMesh()->getTexIDs().size(); i++)
	{
		glActiveTexture(GL_TEXTURE3 + i);
		glBindTexture(GL_TEXTURE_2D, terrain->getTerrainMesh()->getTexIDs()[i]);
		
	}
}


void TerrainRenderer::unbindMesh()
{
	glBindVertexArray(NULL);
}
