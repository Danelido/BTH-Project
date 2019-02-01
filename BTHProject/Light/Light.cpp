#include "Light.h"
#include "App/AppSettings.h"
#include "Acceleration/DataStructs.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, const float& radius) :
	m_position(position), m_color(color), m_radius(radius)
{
	if (AppSettings::DEBUG_LAYER())
		setupDebugData();
}

Light::~Light()
{
	if (m_boundary)
		delete m_boundary;

	if (AppSettings::DEBUG_LAYER()) {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
}

void Light::setPosition(const glm::vec3 & position)
{
	m_position = position;
}

void Light::setColor(const glm::vec3 & color)
{
	m_color = color;
}

void Light::setRadius(const float& radius)
{
	m_radius = radius;
}

const glm::vec3 & Light::getPosition() const
{
	return m_position;
}

const glm::vec3 & Light::getColor() const
{
	return m_color;
}

const float& Light::getRadius() const
{
	return m_radius;
}

void Light::setupDebugData()
{
	m_boundary = new AABB();
	m_boundary->halfDimensions.x = m_radius;
	m_boundary->halfDimensions.y = m_radius;
	m_boundary->halfDimensions.z = m_radius;
	m_boundary->center.x = m_position.x;
	m_boundary->center.y = m_position.y;
	m_boundary->center.z = m_position.z;
	
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

const GLuint& Light::getDebugVAO() const
{
	return m_vao;
}