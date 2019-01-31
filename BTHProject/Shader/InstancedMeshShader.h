#ifndef _INSTANCED_MESH_SHADER_H
#define _INSTANCED_MESH_SHADER_H
#include "Shader.h"
#include <GLM/glm.hpp>

class InstancedMeshShader : public Shader
{
public:
	InstancedMeshShader();
	~InstancedMeshShader();

	void setViewMatrix(const glm::mat4& view);
	void setProjectionMatrix(const glm::mat4& proj);
	void setCameraPosition(const glm::vec3& position);

private:
	GLint m_viewMatrixLocation;
	GLint m_projectionMatrixLocation;
	GLint m_cameraPositionLocation;
};

#endif
