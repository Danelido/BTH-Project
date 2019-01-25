#ifndef _TERRAIN_RENDERER_H
#define _TERRAIN_RENDERER_H

#include "Terrain/Terrain.h"
#include "Utility/FPSCamera.h"
#include "Shader/TerrainShader.h"

class TerrainRenderer
{
public:
	TerrainRenderer(const glm::mat4& projectionMatrix);
	~TerrainRenderer();
	void submit(Terrain* terrain);
	void render(const FPSCamera* camera);

private:
	void bindMesh(Terrain* terrain, const FPSCamera* camera);
	void unbindMesh();
private:
	TerrainShader* m_terrainShader;

	std::vector<Terrain*> m_terrains;
};


#endif
