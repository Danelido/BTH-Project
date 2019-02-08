#ifndef _TERRAIN_H
#define _TERRAIN_H
#include "Parser/ParserData.h"
#include "Graphics/Loader.h"
#include <GLM/glm.hpp>
#include "Acceleration/QuadTree.h"
#include "TerrainChunk.h"
#include "TerrainChunkManager.h"

class Terrain
{
public:
	struct TerrainData {
		float x, y, z;
		float nx, ny, nz;
	};

public:
	Terrain(Loader* loader, TerrainChunkManager* chunkManager, QuadTree* quadtree);
	float getHeight(const glm::vec3& position);
	const glm::mat4& getModelMatrix() const;
	~Terrain();
private:
	void loadHeightsFromHeightmap(unsigned char* textureData, int width, int height, int channels);
	glm::vec3 calculateNormals(TerrainData v1, TerrainData v2, TerrainData v3);
	void smoothNormals();
	void generateTerrain();
	void generateTerrainCells(int nodeX, int nodeY, int cellWidth, int cellHeight, std::vector<GLuint> textureIDs);
private:
	Loader* m_loader;
	QuadTree* m_quadTree;
	ParserData* m_data;
	TerrainChunkManager* m_chunkManager;
	TerrainData* m_heightmapCoords;

	unsigned int m_terrainWidth;
	unsigned int m_terrainHeight;
	
	float m_size;
	float m_heightScaleDivider;

	bool m_loadedSuccessful;
	
	glm::mat4 m_modelMatrix;

};

#endif