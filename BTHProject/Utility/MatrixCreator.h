#ifndef _MATRIX_CREATOR_H
#define _MATRIX_CREATOR_H
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class FPSCamera;

class MatrixCreator
{
public:
	static glm::mat4 createNewModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	static glm::mat4 createNewProjectionMatrix(float fov, float screenWidth, float screenHeight, float near, float far);
};

#endif
