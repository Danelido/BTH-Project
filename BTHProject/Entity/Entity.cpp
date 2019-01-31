#include "Entity.h"
#include "Utility/MatrixCreator.h"
#include "Acceleration/Datastructs.h"
#include "App/AppSettings.h"
Entity::Entity(Mesh* mesh, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation) :
	m_instancedMesh(nullptr), m_mesh(mesh), m_position(position), m_scale(scale), m_rotation(rotation), m_isInstanced(false)
{
	m_boundary = new AABB();
	updateBoundary();
	updateModelMatrix();
}

Entity::Entity(InstancedMesh * instancedMesh, const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotation) :
	m_instancedMesh(instancedMesh), m_mesh(nullptr), m_position(position), m_scale(scale), m_rotation(rotation), m_isInstanced(true)
{
	m_boundary = new AABB();
	updateBoundary();
	updateModelMatrix();
}

Entity::~Entity()
{
	delete m_boundary;
}

void Entity::setPosition(const glm::vec3 & position)
{
	m_position = position;
	updateBoundary();
	updateModelMatrix();
}

void Entity::setRotation(const glm::vec3 & rotation)
{
	m_rotation = rotation;
	updateBoundary();
	updateModelMatrix();
}

void Entity::setScale(const glm::vec3 & scale)
{
	m_scale = scale;
	updateBoundary();
	updateModelMatrix();
}

void Entity::updateBoundary()
{
	m_boundary->halfDimensions.x = m_scale.x;
	m_boundary->halfDimensions.y = m_scale.y;
	m_boundary->halfDimensions.z = m_scale.z;
	
	m_boundary->center.x = m_position.x;
	m_boundary->center.y = m_position.y + m_boundary->halfDimensions.y;
	m_boundary->center.z = m_position.z;
	if (AppSettings::QUADTREE_DBG() && !m_hasBeenSetUp) {
		
		m_hasBeenSetUp = true;

		float data[24 * 3] =
		{
			// FIRST RECTANGLE
			// 1
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,


			// 2
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,


			// 3
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,


			// 4
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,



			// 5
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,


			// 6
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			// 7
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,


			// 8
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			// SECOND RECTANGLE
			// 1
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,


			// 2
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,


			// 3
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,


			// 4
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,



			// 5
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,


			// 6
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			// 7
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,


			// 8
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			// CONNECTORS
			// First to first
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			// Second to second
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z - m_boundary->halfDimensions.z,

			// Third to third
			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,

			m_boundary->center.x - m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,

			// Fourth to fourth
			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y - m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,

			m_boundary->center.x + m_boundary->halfDimensions.x,
			m_boundary->center.y + m_boundary->halfDimensions.y,
			m_boundary->center.z + m_boundary->halfDimensions.z,
		};

	

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}

GLuint Entity::getDebugLineVao()
{
	return m_vao;
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

 AABB& Entity::getBoundary() const
{
	return *m_boundary;
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
