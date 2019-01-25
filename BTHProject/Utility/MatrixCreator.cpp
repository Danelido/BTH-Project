#include "MatrixCreator.h"
#include "FPSCamera.h"

glm::mat4 MatrixCreator::createNewModelMatrix(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale)
{
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, position);
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	matrix = glm::scale(matrix, scale);
	return matrix;

}

glm::mat4 MatrixCreator::createNewProjectionMatrix(float fov, float screenWidth, float screenHeight, float near, float far)
{
	return glm::perspective(glm::radians(fov), screenWidth / screenHeight, near, far);
}
