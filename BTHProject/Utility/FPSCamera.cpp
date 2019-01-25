#include "FPSCamera.h"
#include "Input.h"
#include "App/AppSettings.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

FPSCamera::FPSCamera(const glm::vec3& position)
{
	m_position = position;
	m_pitch = 0.f;
	m_yaw = 0.f;
	// Direction (0.0f, 0.0f, -1.0f)
	m_direction = glm::vec3(glm::sin(glm::radians(m_yaw)), 0, -glm::cos(glm::radians(m_yaw)));
	m_sensitivity = 0.1f;
	m_cameraSpeed = 5.f;
	m_cameraActive = true;
	m_freeLook = true;
	snapMouseToMiddle();
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::update(float dt)
{
	if (Input::isWindowFocused())
	{
		additionalControls();
		if (m_cameraActive)
		{
			lookAround(dt);
			move(dt);
			updateViewMatrix();
		}
	}
}

const glm::mat4& FPSCamera::getViewMatrix() const
{
	return m_viewMatrix;
}

const glm::vec3 & FPSCamera::getPosition() const
{
	return this->m_position;
}

void FPSCamera::additionalControls()
{
	if (Input::isKeyPressed(GLFW_KEY_C))
	{
		if (m_cameraActive)
		{
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_cameraActive = false;
		}
		else
		{
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			snapMouseToMiddle();
			m_cameraActive = true;
		}
	}
}

void FPSCamera::lookAround(float dt)
{
	glfwGetCursorPos(glfwGetCurrentContext(),&m_currentMousePos.x, &m_currentMousePos.y);
	glm::vec2 vel = m_currentMousePos - m_lastMousePos;
	snapMouseToMiddle();

	m_yaw += vel.x * m_sensitivity;
	m_pitch += vel.y * m_sensitivity;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	else if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	m_direction.x = glm::sin(glm::radians(m_yaw));
	m_direction.z = -glm::cos(glm::radians(m_yaw));
}

void FPSCamera::move(float dt)
{
	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		m_position.x += m_direction.x * m_cameraSpeed * dt;
		m_position.z += m_direction.z * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_position.x += m_direction.z * m_cameraSpeed * dt;
		m_position.z -= m_direction.x * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_position.x -= m_direction.x * m_cameraSpeed * dt;
		m_position.z -= m_direction.z * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		m_position.x -= m_direction.z * m_cameraSpeed * dt;
		m_position.z += m_direction.x * m_cameraSpeed * dt;
	}
	
	if (Input::isKeyHeldDown(GLFW_KEY_SPACE) && m_freeLook)
	{
		m_position.y += m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_LEFT_CONTROL) && m_freeLook)
	{
		m_position.y -= m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_LEFT_SHIFT))
	{
		m_cameraSpeed = 50.f;
	}
	else
	{
		m_cameraSpeed = 5.f;
	}
}

void FPSCamera::setY(float y)
{
	m_position.y = y;
}

void FPSCamera::snapMouseToMiddle()
{
	glfwSetCursorPos(glfwGetCurrentContext(), (float)AppSettings::SRCWIDTH() / 2.f, (float)AppSettings::SRCHEIGHT() / 2.f);
	glfwGetCursorPos(glfwGetCurrentContext(), &m_currentMousePos.x, &m_currentMousePos.y);
	m_lastMousePos = m_currentMousePos;
}

void FPSCamera::updateViewMatrix()
{
	m_viewMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(m_pitch), glm::vec3(1.f, 0.f, 0.f));
	m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(m_yaw), glm::vec3(0.f, 1.f, 0.f));
	//m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(m_roll), glm::vec3(0.f, 0.f, 1.f));
	m_viewMatrix = glm::translate(m_viewMatrix, -m_position);
}

void FPSCamera::setFreeLook(bool b)
{
	m_freeLook = b;
}

const bool& FPSCamera::isActive() const
{
	return m_cameraActive;
}