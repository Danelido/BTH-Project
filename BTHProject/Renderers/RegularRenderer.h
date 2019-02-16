#ifndef _REGULAR_RENDERER_H
#define _REGULAR_RENDERER_H
#include "Entity/Entity.h"
#include <map>
#include <vector>
#include "Utility/FPSCamera.h"
#include "Meshes/Mesh.h"
#include "Shader/MeshShader.h"
#include "Shader/ShadowMapShader.h"
#define MESH_VECTOR_RESERVE_SIZE 1000

class RegularRenderer
{
public:
	RegularRenderer(const glm::mat4& projectionMatrix);
	~RegularRenderer();
	void submit(Entity* entity);
	void passAllEntitiesForShadowMapping(std::vector<Entity*>* allEntities);
	void render(const FPSCamera * activeCamera, const FPSCamera* mainCamera);
	void shadowMapPass(ShadowMapShader* shader);

private:
	void bindMesh(Mesh* mesh, const FPSCamera* camera);
	void updateModelMatrix(Entity* entity);
	void unbindMesh(Mesh* mesh);
private:
	std::map<Mesh*, std::vector<Entity*>> m_meshes;
	std::map<Mesh*, std::vector<Entity*>>::iterator m_meshIterator;

	std::vector<Entity*>* m_entitiesShadowPassVector; // will point to the vector in the manager class

	MeshShader* m_meshShader;

};

#endif
