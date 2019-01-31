#ifndef _QUAD_TREE_DEBUG_SHADER
#define _QUAD_TREE_DEBUG_SHADER
#include "Shader.h"
#include <GLM/glm.hpp>

class QuadTreeDebugShader : public Shader
{

public:
	QuadTreeDebugShader();
	~QuadTreeDebugShader();

	void setModelMatrix(const glm::mat4& model);
	void setViewMatrix(const glm::mat4& view);
	void setProjectionMatrix(const glm::mat4& proj);
	void setColor(const glm::vec3& color);
private:
	GLint m_modelMatrixLocation;
	GLint m_viewMatrixLocation;
	GLint m_projectionMatrixLocation;
	GLint m_colorLocation;
};


#endif