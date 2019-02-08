#ifndef _INSTANCED_RENDERER_H
#define _INSTANCED_RENDERER_H
#include "Entity/Entity.h"
#include <map>
#include <vector>
#include "Utility/FPSCamera.h"
#include "Shader/InstancedMeshShader.h"

#define MESH_VECTOR_RESERVE_SIZE 1000

class InstancedRenderer
{
public:
	InstancedRenderer(const glm::mat4& projectionMatrix);
	~InstancedRenderer();
	void submit(Entity* entity);
	void render(const FPSCamera* activeCamera, const FPSCamera* mainCamera);

private:
	void begin(InstancedMesh* mesh, size_t quantity);
	void prepare(Entity* entity);
	void end();
	void flush(InstancedMesh* mesh, size_t quantity, const FPSCamera* camera);

	void bindMesh(InstancedMesh* mesh, const FPSCamera* camera);
	void unbindMesh(InstancedMesh* mesh);

private:
	std::map<InstancedMesh*, std::vector<Entity*>> m_instancedMeshes;
	std::map<InstancedMesh*, std::vector<Entity*>>::iterator m_instancedMeshIterator;
	InstancedMeshShader* m_instancedShader;
	std::vector<glm::mat4> m_modelMatrices;
};

#endif