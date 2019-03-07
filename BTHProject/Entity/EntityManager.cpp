#include "EntityManager.h"
#include "Utility/RandomNum.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/projection.hpp>
#include <iostream>

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

void EntityManager::checkMousePicking(const glm::vec3 & cameraPos, const glm::vec3 & ray, float maxRayDistance, std::vector<QuadTreeObject*>& objects)
{
	if (m_quadtree != nullptr && !m_ignoreQuadTree)
	{
		// Pointing to the selected entity
		// Used for comparing distance towards
		// camera if more than one 
		// intersects with the ray
		Entity* selectedEntity = nullptr;

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->getEntity() != nullptr) {

				Entity* entity = objects[i]->getEntity();
				AABB& boundary = objects[i]->getEntity()->getBoundary();
				XYZ& temp = boundary.center;
				glm::vec3 center = glm::vec3(temp.x, temp.y, temp.z);
			
				// Vector from orgin to center of sphere
				glm::vec3 oc = center - cameraPos;

				// If there was an intersection last iteration
				// compare which is closest to the camera
				if (selectedEntity != nullptr)
				{
					AABB& last_boundary = selectedEntity->getBoundary();
					XYZ& last_temp = last_boundary.center;
					glm::vec3 last_center = glm::vec3(last_temp.x, last_temp.y, last_temp.z);

					float last_lengthToObject = glm::length(last_center - cameraPos);
					float current_lengthToObject = glm::length(oc);
					
					// compare
					if (last_lengthToObject < current_lengthToObject) {
						entity->setSelected(false);
						continue;
					}
				}

				// Our t value that is going to be somewhere in the middle of the part of the ray that is in 
				// the sphere
				float t = glm::dot(oc, ray);

				// Get the point coordinates
				glm::vec3 pointOnT = cameraPos + t * ray;

				// The length of the vector from the point to the center
				float y = glm::length(pointOnT - center);

				if (y > boundary.getLargestDimension())
				{	
					entity->setSelected(false);
					continue;
				}

				selectedEntity = entity;
			}
		}
		
		if (selectedEntity != nullptr)
			selectedEntity->setSelected(true);

	}



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

	masterRenderer->passAllEntitiesForShadowMapping(&m_entities);

}

void EntityManager::setIgnoreQuadTree(bool b)
{
	m_ignoreQuadTree = b;
}

const unsigned int & EntityManager::entitiesProcessed() const
{
	return m_processedEntities;
}