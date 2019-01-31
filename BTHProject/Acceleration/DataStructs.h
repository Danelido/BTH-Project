#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H
#include <GLM/glm.hpp>
#include <Entity/Entity.h>
#include <Terrain/TerrainChunk.h>
struct XZ
{
	float x;
	float z;

	XZ()
	{
		x = -1.f;
		z = -1.f;
	}

	XZ(float x, float z)
	{
		this->x = x;
		this->z = z;
	}

};

struct AABB
{
	XZ center;
	float halfDimension;

	AABB()
	{
		this->halfDimension = 0.f;
	}

	AABB(XZ center, float halfDimension)
	{
		this->center = center;
		this->halfDimension = halfDimension;
	}

	bool containsPoint(XZ point)
	{
		if (point.x >= center.x - halfDimension && point.x <= center.x + halfDimension
			&& point.z >= center.z - halfDimension && point.z <= center.z + halfDimension)
		{
			return true;
		}

		return false;
	}

	bool containsEntity(Entity* entity)
	{
		if (entity->getPosition().x >= center.x - halfDimension && entity->getPosition().x < center.x + halfDimension
			&& entity->getPosition().z >= center.z - halfDimension && entity->getPosition().z < center.z + halfDimension)
		{
			return true;
		}

		return false;
	}

	bool containsTerrainChunk(TerrainChunk* chunk)
	{
		if (chunk->getBoundary().center.x >= center.x - halfDimension && chunk->getBoundary().center.x <= center.x + halfDimension
			&& chunk->getBoundary().center.z >= center.z - halfDimension && chunk->getBoundary().center.z <= center.z + halfDimension)
		{
			return true;
		}

		return false;
	}

	bool intersectsAABB(AABB other)
	{
		if (other.center.x >= center.x - halfDimension && other.center.x <= center.x + halfDimension
			&& other.center.z >= center.z - halfDimension && other.center.z <= center.z + halfDimension)
		{
			return true;
		}

		return false;
	}

	glm::vec3 getVertexP(const glm::vec3& normal)
	{
		glm::vec3 ans = glm::vec3(center.x - halfDimension, 0.f , center.z - halfDimension);

		if (normal.x > 0)
			ans.x += halfDimension * 2.f;

		if (normal.z > 0)
			ans.z += halfDimension * 2.f;

		return ans;
	}

	glm::vec3 getVertexN(const glm::vec3& normal)
	{
		glm::vec3 ans = glm::vec3(center.x - halfDimension, 0.f, center.z - halfDimension);

		if (normal.x < 0)
			ans.x += halfDimension * 2.f;

		if (normal.z < 0)
			ans.z += halfDimension * 2.f;

		return ans;
	}

};


#endif
