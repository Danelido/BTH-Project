#include "EntityManager.h"
#include "Utility/RandomNum.h"
#include <GLM/glm.hpp>

EntityManager::EntityManager()
{
	m_entities.reserve(1000);
	m_processedEntities = 0;
	m_maxProcessingDistance = 20.f;
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

void EntityManager::update(float dt, const FPSCamera* camera, MasterRenderer* masterRenderer)
{
	m_processedEntities = 0;
	for (size_t i = 0; i < m_entities.size(); i++)
	{	
		float distance = glm::distance(camera->getPosition(), m_entities[i]->getPosition());
		if (distance < m_maxProcessingDistance)
		{
			masterRenderer->submitEntity(m_entities[i]);
			m_processedEntities++;
		}

	}
}

void EntityManager::setMaxProcessingDistance(float distance)
{
	m_maxProcessingDistance = distance;
}

const unsigned int & EntityManager::entitiesProcessed() const
{
	return m_processedEntities;
}