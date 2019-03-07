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
	void setCameraPosition(const glm::vec3& position);
	void setProjectionMatrix(const glm::mat4& proj);
	void setShininess(const float& shininess);
	void setSelected(const bool& condition);
private:
	GLint m_modelMatrixLocation;
	GLint m_viewMatrixLocation;
	GLint m_projectionMatrixLocation;
	GLint m_cameraPositionLocation;
	GLint m_shininessLocation;
	GLint m_selectedLocation;
};

#endif
