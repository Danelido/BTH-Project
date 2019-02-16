#ifndef _TERRAIN_RENDERER_H
#define _TERRAIN_RENDERER_H

#include "Terrain/Terrain.h"
#include "Utility/FPSCamera.h"
#include "Shader/TerrainShader.h"
#include "Shader/ShadowMapShader.h"

class TerrainRenderer
{
public:
	TerrainRenderer(const glm::mat4& projectionMatrix);
	~TerrainRenderer();
	void submit(TerrainChunk* chunk);
	void render(const FPSCamera* activeCamera, const FPSCamera* mainCamera);
	void shadowMapPass(ShadowMapShader* shader);

private:
	void bindMesh(TerrainChunk* chunk);
	void unbindMesh();
private:
	TerrainShader* m_terrainShader;
	glm::mat4 m_modelMatrix;
	std::vector<TerrainChunk*> m_chunks;
};


#endif
