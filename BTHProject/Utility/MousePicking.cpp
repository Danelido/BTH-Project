#include "MousePicking.h"
#include "Input.h"
#include "App/AppSettings.h"

MousePicking::MousePicking(const FPSCamera * camera) :
	m_camera(camera)
{
}

MousePicking::~MousePicking()
{
}

glm::vec3 MousePicking::getRay()
{

	glm::vec2 mouse = Input::getMousePosition();

	// NDC
	float x = (2.0f * mouse.x) / AppSettings::SRCWIDTH() - 1.0f;
	float y = 1.0f - (2.0f * mouse.y) / AppSettings::SRCHEIGHT();
	
	// Homogeneous clip space
	float z = -1.0f;
	float w = 1.0f;
	glm::vec4 hcs = glm::vec4(x, y, z, w);

	// Viewspace
	glm::vec4 ray = glm::inverse(m_camera->getProjectionMatrix()) * hcs;
	// Only the xy needed to be unprojected so re-add the z
	ray.z = -1.0f;
	ray.w = 0.0f;

	// World space
	ray = glm::inverse(m_camera->getViewMatrix()) * ray;

	glm::vec3 mouseWorldPosition = glm::normalize(glm::vec3(ray.x, ray.y, ray.z));

	return mouseWorldPosition;
}
