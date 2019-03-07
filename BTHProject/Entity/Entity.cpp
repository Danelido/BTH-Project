#include "Entity.h"
#include "Utility/MatrixCreator.h"
#include "Acceleration/Datastructs.h"
#include "App/AppSettings.h"

Entity::Entity(Mesh* mesh, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation) :
	m_instancedMesh(nullptr), m_mesh(mesh), m_position(position), m_scale(scale), m_rotation(rotation), m_isInstanced(false),
	m_selected(false)
{
	m_boundary = new AABB();
	updateBoundary();
	updateModelMatrix();
}

Entity::Entity(InstancedMesh * instancedMesh, const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotation) :
	m_instancedMesh(instancedMesh), m_mesh(nullptr), m_position(position), m_scale(scale), m_rotation(rotation), m_isInstanced(true),
	m_selected(false)
{
	m_boundary = new AABB();
	updateBoundary();
	updateModelMatrix();
}

Entity::~Entity()
{
	delete m_boundary;

	if (AppSettings::DEBUG_LAYER()) 
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

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
	m_updateBoundaryDebugLines = true;
	updateModelMatrix();
}

void Entity::setBoundaryManually(float width, float height, float depth)
{
	m_updateBoundaryDebugLines = true;
	m_hasManuallySetBoundary = true;
	m_boundary->halfDimensions.x = width / 2.f;
	m_boundary->halfDimensions.y = height / 2.f;
	m_boundary->halfDimensions.z = depth / 2.f;

	m_boundary->center.x = m_position.x;
	m_boundary->center.y = m_position.y + height / 2.f;
	m_boundary->center.z = m_position.z;

	updateBoundary();
}

void Entity::setSelected(bool condition)
{
	m_selected = condition;
}

void Entity::updateBoundary()
{
	if (!m_hasManuallySetBoundary)
	{
		if (m_rotation.x != 0.f || m_rotation.y != 0.f || m_rotation.z != 0.f) {
			float largestScale = m_scale.x;

			largestScale = (largestScale > m_scale.y) ? largestScale : m_scale.y;
			largestScale = (largestScale > m_scale.z) ? largestScale : m_scale.z;

			largestScale *= 1.75f;

			m_boundary->halfDimensions.x = largestScale;
			m_boundary->halfDimensions.y = largestScale;
			m_boundary->halfDimensions.z = largestScale;

		}
		else
		{
			m_boundary->halfDimensions.x = m_scale.x;
			m_boundary->halfDimensions.y = m_scale.y;
			m_boundary->halfDimensions.z = m_scale.z;
		}

		m_boundary->center.x = m_position.x;
		m_boundary->center.y = m_position.y;
		m_boundary->center.z = m_position.z;
	}
	
	
	
	if (AppSettings::DEBUG_LAYER() && !m_hasBeenSetUp || m_updateBoundaryDebugLines) {
		
		m_hasBeenSetUp = true;
		m_updateBoundaryDebugLines = false;
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

	
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
		}
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

GLuint Entity::getDebugVAO()
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

const bool & Entity::isSelected() const
{
	return m_selected;
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
