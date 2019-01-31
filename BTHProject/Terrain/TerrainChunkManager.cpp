#include "TerrainChunkManager.h"
#include "Renderers/MasterRenderer.h"
TerrainChunkManager::TerrainChunkManager()
{
}

TerrainChunkManager::~TerrainChunkManager()
{
	for (size_t i = 0; i < m_chunks.size(); i++)
		delete m_chunks[i];
}

void TerrainChunkManager::updateChunks(MasterRenderer * renderer, std::vector<QuadTreeObject*>& objects)
{
	if (!m_ignoreQuadTree)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->getChunk() != nullptr) {
				renderer->submitTerrain(objects[i]->getChunk());
				m_processedChunks++;
			}
		}
	}
	else if (m_ignoreQuadTree)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->getChunk() != nullptr) {
				renderer->submitTerrain(objects[i]->getChunk());
				m_processedChunks++;
			}
		}
	}
}

void TerrainChunkManager::addChunk(TerrainChunk * chunk)
{
	m_chunks.emplace_back(chunk);
}

void TerrainChunkManager::setIgnoreQuadTree(bool b)
{
	m_ignoreQuadTree = b;
}

void TerrainChunkManager::reserveVectorMemory(int capacity)
{
	m_chunks.reserve(capacity);
}

const unsigned int & TerrainChunkManager::getNrOfChunksProcessed() const
{
	return m_processedChunks;
}
