#include "Camera.h"

Camera::Camera() :
	m_position(glm::vec3(0.f,0.f,0.f)),
	m_front(glm::vec3(0.f, 0.f, 1.f)),
	m_up(glm::vec3(0.f, 1.f, 0.f)),
	m_right(glm::vec3(-1.f, 0.f, 0.f)),
	m_worldUp(glm::vec3(0.f, 1.f, 0.f)),
	m_pitch(0.f),
	m_yaw(-90.f),
	m_movementSpeed(5.f),
	m_mouseSensitivity(0.1f),
	m_freeLook(true)
{
}

Camera::~Camera()
{
}

void Camera::update(float dt)
{
}

void Camera::setY(float y)
{
}

void Camera::setFreeLook(bool b)
{
}

const glm::mat4 & Camera::getViewMatrix() const
{
	// TODO: insert return statement here
	return glm::mat4(1.f);
}

const glm::vec3 & Camera::getPosition() const
{
	// TODO: insert return statement here
	return glm::vec3(1.f);
}

const glm::vec3 & Camera::getCameraUp() const
{
	// TODO: insert return statement here
	return glm::vec3(1.f);
}

void Camera::snapMouseToMiddle()
{
}

void Camera::movement()
{
}

void Camera::mouseLook()
{
}

void Camera::updateVectors()
{
	//m_front.x = glm::cos(glm::radians(m_))
}
