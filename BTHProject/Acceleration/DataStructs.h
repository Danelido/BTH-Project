#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H
#include <GLM/glm.hpp>
#include <Entity/Entity.h>
#include <Terrain/TerrainChunk.h>
#include <Light/Light.h>


struct XYZ
{
	float x;
	float y;
	float z;

	XYZ()
	{
		x = -1.f;
		y = -1.f;
		z = -1.f;
	}

	XYZ(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

};


struct AABB
{
	XYZ center;
	XYZ halfDimensions;

	float vertexPN_scalar = 2.f;

	AABB()
	{
		this->halfDimensions.x = 0.f;
		this->halfDimensions.y = 0.f;
		this->halfDimensions.z = 0.f;
	}

	AABB(XYZ center, float halfDimension)
	{
		this->center = center;
		this->halfDimensions.x = halfDimension;
		this->halfDimensions.y = halfDimension;
		this->halfDimensions.z = halfDimension;
	}

	AABB(XYZ center, float halfDimensionX, float halfDimensionY, float halfDimensionZ)
	{
		this->center = center;
		this->halfDimensions.x = halfDimensionX;
		this->halfDimensions.y = halfDimensionY;
		this->halfDimensions.z = halfDimensionZ;
	}

	bool containsEntity(Entity* entity)
	{
		if (entity->getPosition().x >= center.x - this->halfDimensions.x && entity->getPosition().x < center.x + this->halfDimensions.x
			&& entity->getPosition().z >= center.z - this->halfDimensions.z && entity->getPosition().z < center.z + this->halfDimensions.z)
		{
			return true;
		}

		return false;
	}

	bool containsTerrainChunk(TerrainChunk* chunk)
	{
		if (chunk->getBoundary().center.x >= center.x - this->halfDimensions.x && chunk->getBoundary().center.x <= center.x + this->halfDimensions.x
			&& chunk->getBoundary().center.z >= center.z - this->halfDimensions.z && chunk->getBoundary().center.z <= center.z + this->halfDimensions.z)
		{
			return true;
		}

		return false;
	}

	bool containsLight(Light* light)
	{
		if (light->getPosition().x + light->getRadius() >= center.x - this->halfDimensions.x && light->getPosition().x - light->getRadius() <= center.x + this->halfDimensions.x
			&& light->getPosition().z + light->getRadius() >= center.z - this->halfDimensions.z && light->getPosition().z - light->getRadius() <= center.z + this->halfDimensions.z)
		{
			return true;
		}

		return false;
	}

	bool intersectsAABB(AABB other)
	{
		if (other.center.x >= center.x - this->halfDimensions.x && other.center.x <= center.x + this->halfDimensions.x
			&& other.center.z >= center.z - this->halfDimensions.z && other.center.z <= center.z + this->halfDimensions.z)
		{
			return true;
		}

		return false;
	}

	glm::vec3 getVertexP(const glm::vec3& normal)
	{
		glm::vec3 ans = glm::vec3(center.x - this->halfDimensions.x, center.y - this->halfDimensions.y, center.z - this->halfDimensions.z);

		if (normal.x > 0)
			ans.x += this->halfDimensions.x * vertexPN_scalar;

		if (normal.y > 0)
			ans.y += this->halfDimensions.y * vertexPN_scalar;

		if (normal.z > 0)
			ans.z += this->halfDimensions.z * vertexPN_scalar;

		return ans;
	}

	glm::vec3 getVertexN(const glm::vec3& normal)
	{
		glm::vec3 ans = glm::vec3(center.x - this->halfDimensions.x, center.y - this->halfDimensions.y, center.z - this->halfDimensions.z);

		if (normal.x < 0)
			ans.x += this->halfDimensions.x * vertexPN_scalar;

		if (normal.y < 0)
			ans.y += this->halfDimensions.y * vertexPN_scalar;

		if (normal.z < 0)
			ans.z += this->halfDimensions.z * vertexPN_scalar;

		return ans;
	}

	const float getLargestDimension()
	{
		float largestDim = glm::max(halfDimensions.x, halfDimensions.y);
		largestDim = glm::max(largestDim, halfDimensions.z);

		return largestDim;
	}
};


#endif
