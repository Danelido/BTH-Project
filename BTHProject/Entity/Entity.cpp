#include "Entity.h"
#include "Utility/MatrixCreator.h"

Entity::Entity(Mesh* mesh, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation) :
	m_instancedMesh(nullptr), m_mesh(mesh), m_position(position), m_scale(scale), m_rotation(rotation), m_isInstanced(false)
{
	updateModelMatrix();
}

Entity::Entity(InstancedMesh * instancedMesh, const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotation) :
	m_instancedMesh(instancedMesh), m_mesh(nullptr), m_position(position), m_scale(scale), m_rotation(rotation), m_isInstanced(true)
{
	updateModelMatrix();
}

Entity::~Entity()
{
}

void Entity::setPosition(const glm::vec3 & position)
{
	m_position = position;
	updateModelMatrix();
}

void Entity::setRotation(const glm::vec3 & rotation)
{
	m_rotation = rotation;
	updateModelMatrix();
}

void Entity::setScale(const glm::vec3 & scale)
{
	m_scale = scale;
	updateModelMatrix();
}

const glm::vec3 & Entity::getPosition() const
{
	return m_position;
}

const glm::vec3 & Entity::getRotation() const
{
	return m_rotation;
}

const glm::vec3 & Entity::getScale() const
{
	return m_scale;
}

const glm::mat4 & Entity::getModelMatrix() const
{
	return m_modelMatrix;
}

const bool & Entity::isInstanced() const
{
	return m_isInstanced;
}

Mesh * Entity::getMesh()
{
	return m_mesh;
}

InstancedMesh * Entity::getInstancedMesh()
{
	return m_instancedMesh;
}

void Entity::updateModelMatrix()
{
	m_modelMatrix = MatrixCreator::createNewModelMatrix(m_position, m_rotation, m_scale);
}
