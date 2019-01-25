#ifndef _TERRAIN_H
#define _TERRAIN_H
#include "Parser/ParserData.h"
#include "Graphics/Loader.h"
#include <GLM/glm.hpp>

class Terrain
{
public:
	struct TerrainData {
		float x, y, z;
		float nx, ny, nz;
	};

public:
	Terrain(Loader* loader);
	Mesh* getTerrainMesh();
	float getHeight(const glm::vec3& position);
	const glm::mat4& getModelMatrix() const;
	~Terrain();
private:
	void loadHeightsFromHeightmap(unsigned char* textureData, int width, int height, int channels);
	void setTerrainCoordinates();
	glm::vec3 calculateNormals(TerrainData v1, TerrainData v2, TerrainData v3);
	void smoothNormals();
	void generateTerrain();
private:
	bool m_loadedSuccessful;
	Loader* m_loader;
	Mesh* m_terrainMesh;
	ParserData* m_data;

	unsigned int m_terrainWidth;
	unsigned int m_terrainHeight;
	float m_size;
	float m_heightScaleDivider;

	TerrainData* m_heightmapCoords;
	glm::mat4 m_modelMatrix;

};

#endif