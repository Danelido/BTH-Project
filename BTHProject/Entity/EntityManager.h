#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H
#include "Entity.h"
#include <vector>
#include "Renderers/MasterRenderer.h"
#include "Acceleration/Quadtree.h"


class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void add(Entity* entity);
	void add(Mesh* mesh, const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.f), const glm::vec3& rotation = glm::vec3(0.f));
	void add(InstancedMesh* instancedMesh, const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.f), const glm::vec3& rotation = glm::vec3(0.f));
	void registerQuadtree(QuadTree* quadtree);
	void checkMousePicking(const glm::vec3& cameraPos, const glm::vec3& ray, std::vector<QuadTreeObject*>& objects);
	void update(float dt, const FPSCamera* camera, MasterRenderer* masterRenderer, std::vector<QuadTreeObject*>& objects);
	void setIgnoreQuadTree(bool b);
	const unsigned int& entitiesProcessed() const;
private:
	std::vector<Entity*> m_entities;
	unsigned int m_processedEntities;
	QuadTree* m_quadtree;
	bool m_ignoreQuadTree;
};

#endif
