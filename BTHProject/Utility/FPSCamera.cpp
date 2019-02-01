#include "FPSCamera.h"
#include "Input.h"
#include "App/AppSettings.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>

FPSCamera::FPSCamera(const glm::vec3& position)
{
	m_position = position;
	m_pitch = 0.f;
	m_yaw = 0.f;
	m_walkDirection = glm::vec3(0.f, 0.f, 1.0f);
	m_right = glm::vec3(1.f);
	m_lookDirection = m_walkDirection;
	m_sensitivity = 0.1f;
	m_cameraSpeed = 5.f;
	m_cameraActive = true;
	m_freeLook = true;
	m_debugCamera = false;
	for (size_t i = 0; i < 4; i++)
	{
		m_nearPlanePoints[i] = glm::vec4(0.f);
		m_farPlanePoints[i] = glm::vec4(0.f);
	}
	snapMouseToMiddle();
	calculateFrustumPlaneAttributes();
}

FPSCamera::~FPSCamera()
{
	if (AppSettings::DEBUG_LAYER()) {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
}

void FPSCamera::update(float dt)
{
	if (Input::isWindowFocused())
	{
		additionalControls();
		if (m_cameraActive)
		{
			move(dt);
			lookAround(dt);
			calculateCameraUp();
			calculateFrustumPoints();
			calculateFrustumPlanes();
			updateViewMatrix();
		}
		
	}
}

// A Debug function
void FPSCamera::debugDraw()
{
	if (m_debugCamera && AppSettings::DEBUG_LAYER()) {
		calculateDebugData();
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(m_lineData) * 3, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_numVertices * sizeof(m_lineData) * 3, m_lineData);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);


		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, m_numVertices);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}
// A Debug function
void FPSCamera::initDebugMode()
{
	m_debugCamera = true;
	calculateDebugData();
	setupFrustumLinesAndLoadBuffer();
}

bool FPSCamera::insideFrustum(AABB& aabb)
{
	bool res = true;
	for (size_t i = 0; i < 6; i++)
	{
		if (m_planes[i].distance(aabb.getVertexP(m_planes[i].normal)) < 0.f)
			return false;
		else if (m_planes[i].distance(aabb.getVertexN(m_planes[i].normal)) < 0.f)
			res = true;
	}
	return res;
}

bool FPSCamera::insideFrustum(const glm::vec3& pos, float radius)
{
	bool res = true;
	for (size_t i = 0; i < 6; i++)
	{
		if (m_planes[i].distance(pos) < -radius)
			return false;
		else if (m_planes[i].distance(pos) < radius)
			res = true;
	}
	return res;
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
	m_pitch -= vel.y * m_sensitivity;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	else if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	glm::vec3 lookAt = glm::vec3(1.f);
	lookAt.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	lookAt.y = glm::sin(glm::radians(m_pitch));
	lookAt.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));;
	
	m_lookDirection = glm::normalize(lookAt);

	m_walkDirection.x = glm::cos(glm::radians(m_yaw));
	m_walkDirection.z = glm::sin(glm::radians(m_yaw));
}

void FPSCamera::move(float dt)
{
	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		m_position.x += m_walkDirection.x * m_cameraSpeed * dt;
		m_position.z += m_walkDirection.z * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_position.x += m_walkDirection.z * m_cameraSpeed * dt;
		m_position.z -= m_walkDirection.x * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_position.x -= m_walkDirection.x * m_cameraSpeed * dt;
		m_position.z -= m_walkDirection.z * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		m_position.x -= m_walkDirection.z * m_cameraSpeed * dt;
		m_position.z += m_walkDirection.x * m_cameraSpeed * dt;
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

void FPSCamera::calculateCameraUp()
{
	m_right = glm::normalize(glm::cross(m_lookDirection,glm::vec3(0.f, 1.f, 0.f)));
	m_cameraUp = glm::cross(m_right, m_lookDirection);
}

// Note to self: going counter clock-wise, starting at upper left corner
void FPSCamera::calculateFrustumPoints()
{
	glm::vec3 nc = m_position + (m_lookDirection * NEAR_CLIPPING);
	glm::vec3 fc = m_position + (m_lookDirection * FAR_CLIPPING);

	m_nearPlanePoints[0] = nc + (m_cameraUp * (m_nearPlaneHeight / 2.f)) - (m_right * (m_nearPlaneWidth / 2.f));
	m_nearPlanePoints[1] = nc - (m_cameraUp * (m_nearPlaneHeight / 2.f)) - (m_right * (m_nearPlaneWidth / 2.f));
	m_nearPlanePoints[2] = nc - (m_cameraUp * (m_nearPlaneHeight / 2.f)) + (m_right * (m_nearPlaneWidth / 2.f));
	m_nearPlanePoints[3] = nc + (m_cameraUp * (m_nearPlaneHeight / 2.f)) + (m_right * (m_nearPlaneWidth / 2.f));


	m_farPlanePoints[0] = fc + (m_cameraUp * (m_farPlaneHeight / 2.f)) - (m_right * (m_farPlaneWidth / 2.f));
	m_farPlanePoints[1] = fc - (m_cameraUp * (m_farPlaneHeight / 2.f)) - (m_right * (m_farPlaneWidth / 2.f));
	m_farPlanePoints[2] = fc - (m_cameraUp * (m_farPlaneHeight / 2.f)) + (m_right * (m_farPlaneWidth / 2.f));
	m_farPlanePoints[3] = fc + (m_cameraUp * (m_farPlaneHeight / 2.f)) + (m_right * (m_farPlaneWidth / 2.f));
}

// Only needs to be called when fov, near clipping plane or far clipping plane is changed
void FPSCamera::calculateFrustumPlaneAttributes()
{
	m_nearPlaneHeight = NEAR_CLIPPING * glm::tan(glm::radians(FOV / 2.f)) * 2.f;
	m_nearPlaneWidth = m_nearPlaneHeight * (static_cast<float>(AppSettings::SRCWIDTH()) / static_cast<float>(AppSettings::SRCHEIGHT()));

	m_farPlaneHeight = FAR_CLIPPING * glm::tan(glm::radians(FOV / 2.f)) * 2.f;
	m_farPlaneWidth = m_farPlaneHeight * (static_cast<float>(AppSettings::SRCWIDTH()) / static_cast<float>(AppSettings::SRCHEIGHT()));
}

void FPSCamera::calculateFrustumPlanes()
{

	// Notes:
	/*
		top left		= 0
		bottom left		= 1
		bottom right	= 2
		top right		= 3
	*/

	m_planes[TOP].setPoints(m_nearPlanePoints[3], m_nearPlanePoints[0], m_farPlanePoints[0]);
	m_planes[BOTTOM].setPoints(m_nearPlanePoints[1], m_nearPlanePoints[2], m_farPlanePoints[2]);
	m_planes[LEFT].setPoints(m_nearPlanePoints[0], m_nearPlanePoints[1], m_farPlanePoints[0]);
	m_planes[RIGHT].setPoints(m_nearPlanePoints[2], m_nearPlanePoints[3], m_farPlanePoints[2]);
	m_planes[NEARP].setPoints(m_nearPlanePoints[0], m_nearPlanePoints[3], m_nearPlanePoints[2]);
	m_planes[FARP].setPoints(m_farPlanePoints[3], m_farPlanePoints[0], m_farPlanePoints[1]);
}

// A Debug function
void FPSCamera::setupFrustumLinesAndLoadBuffer()
{
	// Debug lines for near and far planes
	m_numVertices = 24;
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(m_lineData), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// A Debug function
void FPSCamera::calculateDebugData()
{
	float data[24 * 3] =
	{
		m_nearPlanePoints[0].x,
		m_nearPlanePoints[0].y,
		m_nearPlanePoints[0].z,

		m_nearPlanePoints[1].x,
		m_nearPlanePoints[1].y,
		m_nearPlanePoints[1].z,

		m_nearPlanePoints[1].x,
		m_nearPlanePoints[1].y,
		m_nearPlanePoints[1].z,

		m_nearPlanePoints[2].x,
		m_nearPlanePoints[2].y,
		m_nearPlanePoints[2].z,

		m_nearPlanePoints[2].x,
		m_nearPlanePoints[2].y,
		m_nearPlanePoints[2].z,

		m_nearPlanePoints[3].x,
		m_nearPlanePoints[3].y,
		m_nearPlanePoints[3].z,

		m_nearPlanePoints[3].x,
		m_nearPlanePoints[3].y,
		m_nearPlanePoints[3].z,

		m_nearPlanePoints[0].x,
		m_nearPlanePoints[0].y,
		m_nearPlanePoints[0].z,

		// from near to far
		m_nearPlanePoints[0].x,
		m_nearPlanePoints[0].y,
		m_nearPlanePoints[0].z,
		// Begin far
		m_farPlanePoints[0].x,
		m_farPlanePoints[0].y,
		m_farPlanePoints[0].z,

		m_farPlanePoints[0].x,
		m_farPlanePoints[0].y,
		m_farPlanePoints[0].z,

		m_farPlanePoints[1].x,
		m_farPlanePoints[1].y,
		m_farPlanePoints[1].z,

		m_farPlanePoints[1].x,
		m_farPlanePoints[1].y,
		m_farPlanePoints[1].z,

		m_farPlanePoints[2].x,
		m_farPlanePoints[2].y,
		m_farPlanePoints[2].z,

		m_farPlanePoints[2].x,
		m_farPlanePoints[2].y,
		m_farPlanePoints[2].z,

		m_farPlanePoints[3].x,
		m_farPlanePoints[3].y,
		m_farPlanePoints[3].z,

		m_farPlanePoints[3].x,
		m_farPlanePoints[3].y,
		m_farPlanePoints[3].z,

		m_farPlanePoints[0].x,
		m_farPlanePoints[0].y,
		m_farPlanePoints[0].z,

		m_farPlanePoints[3].x,
		m_farPlanePoints[3].y,
		m_farPlanePoints[3].z,

		m_nearPlanePoints[3].x,
		m_nearPlanePoints[3].y,
		m_nearPlanePoints[3].z,

		m_nearPlanePoints[1].x,
		m_nearPlanePoints[1].y,
		m_nearPlanePoints[1].z,

		m_farPlanePoints[1].x,
		m_farPlanePoints[1].y,
		m_farPlanePoints[1].z,

		m_nearPlanePoints[2].x,
		m_nearPlanePoints[2].y,
		m_nearPlanePoints[2].z,

		m_farPlanePoints[2].x,
		m_farPlanePoints[2].y,
		m_farPlanePoints[2].z,

	};

	m_lineData = data;

}

void FPSCamera::updateViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_lookDirection, m_cameraUp);
}

void FPSCamera::setFreeLook(bool b)
{
	m_freeLook = b;
}

const glm::vec3& FPSCamera::getCameraUp() const
{
	return m_cameraUp;
}

const bool& FPSCamera::isActive() const
{
	return m_cameraActive;
}