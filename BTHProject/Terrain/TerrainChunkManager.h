#ifndef _TERRAIN_CHUNK_MANAGER_H
#define _TERRAIN_CHUNK_MANAGER_H
#include <vector>
#include <Terrain/TerrainChunk.h>
#include "Acceleration/QuadTreeObject.h"

class MasterRenderer;

class TerrainChunkManager
{
public:
	TerrainChunkManager();
	~TerrainChunkManager();
	void updateChunks(MasterRenderer* renderer, std::vector<QuadTreeObject*>& objects);
	void addChunk(TerrainChunk* chunk);
	void setIgnoreQuadTree(bool b);
	void reserveVectorMemory(int capacity);
	const unsigned int& getNrOfChunksProcessed() const;

private:
	bool m_ignoreQuadTree;
	unsigned int m_processedChunks;
	std::vector<TerrainChunk*> m_chunks;
};



#endif
