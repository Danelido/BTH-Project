#include "EntityManager.h"
#include "Utility/RandomNum.h"
#include <GLM/glm.hpp>

EntityManager::EntityManager()
{
	m_entities.reserve(1000);
	m_processedEntities = 0;
}

EntityManager::~EntityManager()
{
	for (size_t i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i])
			delete m_entities[i];
	}
}

void EntityManager::add(Entity * entity)
{
	m_entities.emplace_back(entity);
}

void EntityManager::add(Mesh * mesh, const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotation)
{
	m_entities.emplace_back(new Entity(mesh, position, scale, rotation));
}

void EntityManager::add(InstancedMesh * instancedMesh, const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotation)
{
	m_entities.emplace_back(new Entity(instancedMesh, position, scale, rotation));
}

void EntityManager::registerQuadtree(QuadTree * quadtree)
{
	m_quadtree = quadtree;
}

void EntityManager::update(float dt, const FPSCamera* camera, MasterRenderer* masterRenderer, std::vector<QuadTreeObject*>& objects)
{
	m_processedEntities = 0;
	
	if (m_quadtree != nullptr && !m_ignoreQuadTree)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->getEntity() != nullptr) {
				masterRenderer->submitEntity(objects[i]->getEntity());
				m_processedEntities++;
			}
		}

	}
	else
	{
		for (size_t i = 0; i < m_entities.size(); i++)
		{
			masterRenderer->submitEntity(m_entities[i]);
			m_processedEntities++;
		}
	}

}

void EntityManager::setIgnoreQuadTree(bool b)
{
	m_ignoreQuadTree = b;
}

const unsigned int & EntityManager::entitiesProcessed() const
{
	return m_processedEntities;
}