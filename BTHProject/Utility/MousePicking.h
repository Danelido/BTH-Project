#ifndef _MOUSE_PICKING_H
#define _MOUSE_PICKING_H
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "FPSCamera.h"

class MousePicking
{
public:
	MousePicking(const FPSCamera* camera);
	~MousePicking();

	glm::vec3 getRay();

private:
	const FPSCamera* m_camera;
};


#endif
