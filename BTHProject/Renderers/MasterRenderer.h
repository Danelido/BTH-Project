#ifndef _MASTER_RENDERER_H
#define _MASTER_RENDERER_H
#include "Meshes/Mesh.h"
#include <map>
#include <vector>
#include "Shader/FBOShader.h"
#include "Entity/Entity.h"
#include "Utility/FPSCamera.h"
#include "Graphics/FBO.h"
#include "Graphics/Quad.h"

#include "RegularRenderer.h"
#include "InstancedRenderer.h"
#include "Terrain/Terrain.h"
#include "Entity/Light.h"
#include "TerrainRenderer.h"

class MasterRenderer
{
public:
	MasterRenderer(const FPSCamera* camera);
	~MasterRenderer();
	
	void submitEntity(Entity* entity);
	void submitLight(Light* light);
	void submitTerrain(Terrain* terrain);
	void setSunPosition(const glm::vec3& position);
	void render();
	
	const size_t& getNumberOfLights() const;

private:
	void renderFBO();
private:
	FBOShader* m_fboShader;
	FBO* m_FBO;
	Quad* m_fboQuad;
	
	glm::mat4 m_projectionMatrix;
	const FPSCamera* m_camera;

	RegularRenderer* m_regularRenderer;
	InstancedRenderer* m_instancedRenderer;
	TerrainRenderer* m_terrainRenderer;
	std::vector<Light*> m_lights;

	glm::vec3 m_sunPosition;
};

#endif
