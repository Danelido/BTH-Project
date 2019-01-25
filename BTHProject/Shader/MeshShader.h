#ifndef _MESH_SHADER_H
#define _MESH_SHADER_H
#include "Shader.h"
#include <GLM/glm.hpp>

class MeshShader : public Shader
{
public:
	MeshShader();
	~MeshShader();

	void setModelMatrix(const glm::mat4& model);
	void setViewMatrix(const glm::mat4& view);
	void setProjectionMatrix(const glm::mat4& proj);
	void setCameraPosition(const glm::vec3& position);
private:
	GLint m_modelMatrixLocation;
	GLint m_viewMatrixLocation;
	GLint m_projectionMatrixLocation;
	GLint m_cameraPositionLocation;

};

#endif
