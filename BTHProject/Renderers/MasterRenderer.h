#ifndef _MASTER_RENDERER_H
#define _MASTER_RENDERER_H
#include <map>
#include <vector>
#include "Meshes/Mesh.h"
#include "Shader/FBOShader.h"
#include "Shader/QuadTreeDebugShader.h"
#include "Graphics/FBO.h"
#include "Graphics/Quad.h"
#include "Entity/Entity.h"
#include "Entity/Light.h"
#include "Utility/FPSCamera.h"
#include "Terrain/Terrain.h"
#include "Acceleration/QuadTree.h"

#include "RegularRenderer.h"
#include "InstancedRenderer.h"
#include "TerrainRenderer.h"

class MasterRenderer
{
public:
	MasterRenderer(FPSCamera* camera);
	~MasterRenderer();
	
	void submitEntity(Entity* entity);
	void submitLight(Light* light);
	void submitTerrain(TerrainChunk* chunk);
	void registerQuadTree(QuadTree* quadTree);
	void setSunPosition(const glm::vec3& position);
	void render();
	void changeCamera(FPSCamera * camera);
	const size_t getNumberOfLights() const;

private:
	void renderFBO();
	void renderQuadTree();
private:
	FBOShader* m_fboShader;
	QuadTreeDebugShader* m_quadTreeDebugShader;
	FBO* m_FBO;
	Quad* m_fboQuad;
	
	glm::mat4 m_projectionMatrix;
	FPSCamera* m_camera;
	FPSCamera* m_activeCamera;

	RegularRenderer* m_regularRenderer;
	InstancedRenderer* m_instancedRenderer;
	TerrainRenderer* m_terrainRenderer;
	std::vector<Light*> m_lights;
	QuadTree* m_quadTree;
	glm::vec3 m_sunPosition;
};

#endif
