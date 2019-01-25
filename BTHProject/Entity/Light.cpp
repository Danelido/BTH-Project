#include "Light.h"


Light::Light(const glm::vec3& position, const glm::vec3& color) :
	m_position(position), m_color(color)
{
}

Light::~Light()
{
}

void Light::setPosition(const glm::vec3 & position)
{
	m_position = position;
}

void Light::setColor(const glm::vec3 & color)
{
	m_color = color;
}

const glm::vec3 & Light::getPosition() const
{
	return m_position;
}

const glm::vec3 & Light::getColor() const
{
	return m_color;
}
