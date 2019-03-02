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
#include "Light/Light.h"
#include "Utility/FPSCamera.h"
#include "Terrain/Terrain.h"
#include "Acceleration/QuadTree.h"

#include "RegularRenderer.h"
#include "InstancedRenderer.h"
#include "TerrainRenderer.h"

#include "Shader/ShadowMapShader.h"

class MasterRenderer
{
public:
	MasterRenderer(FPSCamera* camera);
	~MasterRenderer();
	
	void submitEntity(Entity* entity);
	void submitLight(Light* light);
	void submitTerrain(TerrainChunk* chunk);
	void registerQuadTree(QuadTree* quadTree);
	void passAllEntitiesForShadowMapping(std::vector<Entity*>* allEntities);
	void setSunPosition(const glm::vec3& position);
	void setShadowBiaz(const float& biaz);
	void render();
	void changeCamera(FPSCamera * camera);
	
private:
	void renderFBO();
	void renderQuadTree();
private:
	FBOShader* m_fboShader;
	QuadTreeDebugShader* m_quadTreeDebugShader;
	ShadowMapShader* m_shadowMapShader;
	glm::mat4 m_lightProj;
	glm::mat4 m_lightView;
	glm::mat4 lightTransform;
	FBO* m_FBO;
	Quad* m_fboQuad;
	
	float m_shadowBiaz;
	glm::vec3 m_sunDirection;
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
