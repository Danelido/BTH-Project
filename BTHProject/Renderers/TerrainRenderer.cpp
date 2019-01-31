#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(const glm::mat4 & projectionMatrix)
{
	m_terrainShader = new TerrainShader();
	m_terrainShader->use();
	m_terrainShader->setProjectionMatrix(projectionMatrix);
	m_terrainShader->unuse();

	m_chunks.reserve(5000);
}

TerrainRenderer::~TerrainRenderer()
{
	delete m_terrainShader;
}

void TerrainRenderer::submit(TerrainChunk * terrain)
{
	m_chunks.emplace_back(terrain);
}

void TerrainRenderer::render(const FPSCamera * camera)
{
	if (m_chunks.size() == 0)
		return; 

	m_terrainShader->use();
	m_terrainShader->setCameraPosition(camera->getPosition());
	m_terrainShader->setModelMatrix(glm::mat4(1.f));
	m_terrainShader->setViewMatrix(camera->getViewMatrix());


	for (int i = 0; i < m_chunks[0]->getMesh()->getTexIDs().size(); i++)
	{
		glActiveTexture(GL_TEXTURE3 + i);
		glBindTexture(GL_TEXTURE_2D, m_chunks[0]->getMesh()->getTexIDs()[i]);

	}

	for (size_t j = 0; j < m_chunks.size(); j++) {
		bindMesh(m_chunks[j], camera);
		glDrawElements(GL_TRIANGLES, m_chunks[j]->getMesh()->getIndicesSize(), GL_UNSIGNED_INT, NULL);
		unbindMesh();
	}
	
	m_terrainShader->unuse();

	m_chunks.clear();
}

void TerrainRenderer::bindMesh(TerrainChunk * chunk, const FPSCamera * camera)
{
	glBindVertexArray(chunk->getMesh()->getVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}


void TerrainRenderer::unbindMesh()
{
	glBindVertexArray(NULL);
}
