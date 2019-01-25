#ifndef _REGULAR_RENDERER_H
#define _REGULAR_RENDERER_H
#include "Entity/Entity.h"
#include <map>
#include <vector>
#include "Utility/FPSCamera.h"
#include "Meshes/Mesh.h"
#include "Shader/MeshShader.h"

#define MESH_VECTOR_RESERVE_SIZE 1000

class RegularRenderer
{
public:
	RegularRenderer(const glm::mat4& projectionMatrix);
	~RegularRenderer();
	void submit(Entity* entity);
	void render(const FPSCamera* camera);

private:
	void bindMesh(Mesh* mesh, const FPSCamera* camera);
	void updateModelMatrix(Entity* entity);
	void unbindMesh(Mesh* mesh);
private:
	std::map<Mesh*, std::vector<Entity*>> m_meshes;
	std::map<Mesh*, std::vector<Entity*>>::iterator m_meshIterator;

	MeshShader* m_meshShader;

};

#endif
